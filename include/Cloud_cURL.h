/* A class to facilitate cURL communication with cloud
 * Created by Tong Tin Yi (2023)
 */

/**************************************/
/* all functions return int value for */
/* error handling. value 0 means okay */
/* if value is -1, error occurs       */
/**************************************/

#ifndef CLOUD_CURL_H
#define CLOUD_CURL_H

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>
#include <cstdlib>
#include <algorithm>

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite(ptr,size,nmemb, (FILE *)stream);
    return written;
}

/**
 * @brief WriteCallback gets called by libcurl as soon as there is data received that needs to be saved.
 */
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

/**
 * @brief progress_callback gets called by callback by libcurl instead of its internal equivalent with a frequent internal.
 */
static int progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow){
	if (dltotal != 0)
	{
		printf("%lf / %lf (%lf %%)\n", dlnow, dltotal, dlnow*100.0 / dltotal);
	}
	return 0;
}

/**
 * @brief A class to facilitate cURL communication with cloud
 * @param url is the endpoint to server
 */
class Cloud_cURL
{
    public:
        Cloud_cURL(std::string url);
        virtual ~Cloud_cURL();

        /**
         * @brief Initialize the libcurl
         * @return -1, if error occurs
         */
        bool CURL_init();
        /**
         * @brief Initialize the environment for libcurl
         */
        static void CURL_globalInit()
        {
            curl_global_init(CURL_GLOBAL_ALL);
        };
        /**
         * @brief Release resources aquired by calling curl_global_init
         */
        static void CURL_globalClear()
        {
            curl_global_cleanup();
        };

        /**
         * @brief Send message to the connected endpoint in json dictionary form
         * @param Dict_key is the name of the dictionary
         * @param value is the string dictionary value
         * @return -1, if error occurs
         */
        int sendMsg(std::string Dict_key, std::string value);
        /**
         * @brief Send message to the connected endpoint in json dictionary form
         * @param Dict_key is the name of the dictionary
         * @param value is the boolean dictionary value
         * @return -1, if error occurs
         */
        int sendMsg(std::string Dict_key, bool value);
        /**
         * @brief Send message to the connected endpoint in two json dictionaries
         * @param Dict_key is the name of the dictionary
         * @param value is the string dictionary value
         * @param Dict_key2 is the name of the second dictionary
         * @param value2 is the second string dictionary value
         * @return -1, if error occurs
         */
        int sendMsg(std::string Dict_key, std::string value, std::string Dict_key2, std::string value2);
        /**
         * @brief Send message to the connected endpoint in thress json dictionaries
         * @param Dict_key is the name of the dictionary
         * @param value is the string dictionary value
         * @param Dict_key2 is the name of the second dictionary
         * @param value2 is the second string dictionary value
         * @param Dict_key3 is the name of the third dictionary
         * @param value3 is the third string dictionary value
         * @return -1, if error occurs
         */
        int sendMsg(std::string Dict_key, std::string value, std::string Dict_key2, std::string value2, std::string Dict_key3, std::string value3);
        /**
         * @brief send a json list of image name that found to be faulty and thus not accpeted in database
         * @param faultyimage is the vector of image name in string
         * @return -1, if error occurs
         */
        int sendFaultyImage(std::vector<std::string> faultyimage);

        /**
         * @brief Send an image(jpg) to connected endpoint encoded in base64
         * @param image_path is the path of image to be sent
         * @return -1, if error occurs
         */
        int sendPhoto(std::string image_path);

        /**
         * @brief Download image(jpg) from cloud server
         * @param database_path is the path to store the image
         * @param img_path is the link to cloud location, where the image is at
         * @return -1, if error occurs
         */
        int downloadPhoto(std::string database_path, std::string &img_path);

        /**
         * @brief Read response to request made by visitor
         * @param requestCode is the int code assigned to each request to prevent data race
         * @param m_pair is the pair storing return request Code and response
         * @return -1, if error occurs
         */
        int readRequestStatus(int requestCode, std::pair<int, std::string>& m_pair);
        /**
         * @brief Read message from Smartphone App
         * @param requestCode is the int code assigned to each request to prevent data race
         * @param answerCode is the return request code, after increment
         * @param userToAdd is the vector containing new user images
         * @param userToDelete is the vector containing user that should be removed
         * @param liveStreamOn is true, when App request to turn on live streaming
         * @return -1, if error occurs
         */
        int readMessage(int requestCode, int &answerCode, std::vector<std::string> &userToAdd, std::vector<std::string> &userToDelete, bool &liveStreamOn);

    protected:
        CURL* _curl;

    private:
        /**
         * @brief API_URL is the cloud API url
         */
        std::string API_URL;
};

#endif // CLOUD_CURL_H
