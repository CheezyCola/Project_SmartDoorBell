/* A class to facilitate cURL communication with cloud
 * Created by Tong Tin Yi (2023) based on tutorials/utility_client
 * in github https://github.com/zaphoyd/websocketpp
 */

/**************************************/
/* all functions return int value for */
/* error handling. value 0 means okay */
/* if value is -1, error occurs       */
/**************************************/
#ifndef LIVESTREAM_H
#define LIVESTREAM_H

#include <iostream>
#include <chrono>
#include <thread>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <cstdlib>
#include <map>
#include <string>
#include <sstream>
#include "base64.h"

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>

typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;
typedef websocketpp::client<websocketpp::config::asio_tls_client> client;

static bool closing;

class connection_metadata {
public:
    typedef websocketpp::lib::shared_ptr<connection_metadata> ptr;

    connection_metadata(int id, websocketpp::connection_hdl hdl, std::string uri)
        : m_id(id)
        , m_hdl(hdl)
        , m_status("Connecting")
        , m_uri(uri)
        , m_server("N/A")
    {}

    void on_open(client * c, websocketpp::connection_hdl hdl) {
        m_status = "Open";

        client::connection_ptr con = c->get_con_from_hdl(hdl);
        m_server = con->get_response_header("Server");
    }

    void on_fail(client * c, websocketpp::connection_hdl hdl) {
        m_status = "Failed";

        client::connection_ptr con = c->get_con_from_hdl(hdl);
        m_server = con->get_response_header("Server");
        m_error_reason = con->get_ec().message();
    }

    void on_close(client * c, websocketpp::connection_hdl hdl) {
        m_status = "Closed";
        client::connection_ptr con = c->get_con_from_hdl(hdl);
        std::stringstream s;
        s << "close code: " << con->get_remote_close_code() << " ("
          << websocketpp::close::status::get_string(con->get_remote_close_code())
          << "), close reason: " << con->get_remote_close_reason();
        m_error_reason = s.str();
    }

    void on_message(websocketpp::connection_hdl, client::message_ptr msg) {
        if (msg->get_opcode() == websocketpp::frame::opcode::text) {
            m_messages.push_back("<< " + msg->get_payload());
        } else {
            m_messages.push_back("<< " + websocketpp::utility::to_hex(msg->get_payload()));
        }
    }

    websocketpp::connection_hdl get_hdl() const {
        return m_hdl;
    }

    int get_id() const {
        return m_id;
    }

    std::string get_status() const {
        return m_status;
    }

    void record_sent_message(std::string message) {
        m_messages.push_back(">> " + message);
    }

    friend std::ostream & operator<< (std::ostream & out, connection_metadata const & data);

private:
    int m_id;
    websocketpp::connection_hdl m_hdl;
    std::string m_status;
    std::string m_uri;
    std::string m_server;
    std::string m_error_reason;
    std::vector<std::string> m_messages;
};

inline std::ostream & operator<< (std::ostream & out, connection_metadata const & data) {
    out << "> URI: " << data.m_uri << "\n"
        << "> Status: " << data.m_status << "\n"
        << "> Remote Server: " << (data.m_server.empty() ? "None Specified" : data.m_server) << "\n"
        << "> Error/close reason: " << (data.m_error_reason.empty() ? "N/A" : data.m_error_reason) << "\n";
    out << "> Messages Processed: (" << data.m_messages.size() << ") \n";

    std::vector<std::string>::const_iterator it;
    for (it = data.m_messages.begin(); it != data.m_messages.end(); ++it) {
        out << *it << "\n";
    }

    return out;
}

inline context_ptr on_tls_init(websocketpp::connection_hdl) {
    context_ptr ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
    return ctx;
}

/**
 * @brief A class to manage endpoint connection used in sending frame in LiveStream
 */
class websocket_endpoint {
public:
    websocket_endpoint () : m_next_id(0) {
        m_endpoint.set_access_channels(websocketpp::log::alevel::all);
        m_endpoint.clear_access_channels(websocketpp::log::alevel::all);
        m_endpoint.clear_error_channels(websocketpp::log::elevel::all);

        m_endpoint.set_tls_init_handler(websocketpp::lib::bind(
                                            on_tls_init,
                                            websocketpp::lib::placeholders::_1
                                            ));

        m_endpoint.init_asio();
        m_endpoint.start_perpetual();

        m_thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&client::run, &m_endpoint);
        closing = false;
    }

    ~websocket_endpoint() {
        closing = true;
        m_endpoint.stop_perpetual();

        for (con_list::const_iterator it = m_connection_list.begin(); it != m_connection_list.end(); ++it) {
            if (it->second->get_status() != "Open") {
                // Only close open connections
                continue;
            }

            std::cout << "> Closing connection " << it->second->get_id() << std::endl;

            websocketpp::lib::error_code ec;
            m_endpoint.close(it->second->get_hdl(), websocketpp::close::status::going_away, "", ec);
            if (ec) {
                std::cout << "> Error closing connection " << it->second->get_id() << ": "
                          << ec.message() << std::endl;
            }
        }

        m_thread->join();  
        closing = false;
    }

    int connect(std::string const & uri) {
        websocketpp::lib::error_code ec;
        std::cout << "Connecting to " + uri << std::endl;
        client::connection_ptr con = m_endpoint.get_connection(uri, ec);

        if (ec) {
            std::cout << "> Connect initialization error: " << ec.message() << std::endl;
            return -1;
        }

        int new_id = m_next_id++;
        connection_metadata::ptr metadata_ptr = websocketpp::lib::make_shared<connection_metadata>(new_id, con->get_handle(), uri);
        m_connection_list[new_id] = metadata_ptr;

        con->set_open_handler(websocketpp::lib::bind(
                                  &connection_metadata::on_open,
                                  metadata_ptr,
                                  &m_endpoint,
                                  websocketpp::lib::placeholders::_1
                                  ));
        con->set_fail_handler(websocketpp::lib::bind(
                                  &connection_metadata::on_fail,
                                  metadata_ptr,
                                  &m_endpoint,
                                  websocketpp::lib::placeholders::_1
                                  ));
        con->set_close_handler(websocketpp::lib::bind(
                                   &connection_metadata::on_close,
                                   metadata_ptr,
                                   &m_endpoint,
                                   websocketpp::lib::placeholders::_1
                                   ));
        con->set_message_handler(websocketpp::lib::bind(
                                     &connection_metadata::on_message,
                                     metadata_ptr,
                                     websocketpp::lib::placeholders::_1,
                                     websocketpp::lib::placeholders::_2
                                     ));

        m_endpoint.connect(con);

        return new_id;
    }

    void close(int id, websocketpp::close::status::value code, std::string reason) {
        websocketpp::lib::error_code ec;

        con_list::iterator metadata_it = m_connection_list.find(id);
        if (metadata_it == m_connection_list.end()) {
            std::cout << "> No connection found with id " << id << std::endl;
            return;
        }

        m_endpoint.close(metadata_it->second->get_hdl(), code, reason, ec);
        if (ec) {
            std::cout << "> Error initiating close: " << ec.message() << std::endl;
        }
    }

    bool send(int id, std::string message) {
        bool status = true;
        websocketpp::lib::error_code ec;

        con_list::iterator metadata_it = m_connection_list.find(id);
        if (metadata_it == m_connection_list.end()) {
            std::cout << "> No connection found with id " << id << std::endl;
            status = false;
        }

        m_endpoint.send(metadata_it->second->get_hdl(), message, websocketpp::frame::opcode::text, ec);
        if (ec) {
            std::cout << "> Error sending message: " << ec.message() << std::endl;
            status = false;
        }

        metadata_it->second->record_sent_message(message);
        return status;
    }

    connection_metadata::ptr get_metadata(int id) const {
        con_list::const_iterator metadata_it = m_connection_list.find(id);
        if (metadata_it == m_connection_list.end()) {
            return connection_metadata::ptr();
        } else {
            return metadata_it->second;
        }
    }
private:
    typedef std::map<int,connection_metadata::ptr> con_list;

    client m_endpoint;
    websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;

    con_list m_connection_list;
    int m_next_id;
};

/**
 * @brief A class to facilitate cURL communication with cloud in live streaming frames
 * https://github.com/zaphoyd/websocketpp
 */
class LiveStream
{
    websocket_endpoint *endpoint = new websocket_endpoint();
    int endpoint_id;
public:
    static std::string server_url; // Replace with your server URL
    bool toReconnect = false;
    LiveStream()
    {
        endpoint_id = endpoint->connect(server_url);
        if (endpoint_id != -1) {
            std::cout << "> Created connection with id" << endpoint_id << std::endl;
        }
    };

    ~LiveStream()
    {
        delete endpoint;
        endpoint = nullptr;
    };

    /**
     * @brief Reconnect to cloud endpoint
     */
    void reconnect(){
        if (endpoint_id == 2){
            closing = true;
            toReconnect = true;
            delete endpoint;
            endpoint = new websocket_endpoint();
        }else if (!closing){
            endpoint_id = endpoint->connect(server_url);
            if (endpoint_id != -1) {
                std::cout << "> Created connection with id" << endpoint_id << std::endl;
            }
        }
    }

    /**
     * @brief Send the frame to endpoint, encoded in base64
     * @param frame: frame captured by camera
     */
    void stream(cv::Mat frame)
    {
        if(!closing){
            //Convert the frame to base64
            std::vector<uchar> buffer;
            cv::imencode(".jpg", frame, buffer);
            auto *enc = reinterpret_cast<unsigned char*>(buffer.data());
            std::string frame_base64 = base64_encode(enc, buffer.size());
            if (toReconnect){
                toReconnect = false;
                reconnect();
            }
            else{
                if(!endpoint->send(endpoint_id, frame_base64)){
                    reconnect();
                }
            }
        }
    };
};

#endif // LIVESTREAM_H
