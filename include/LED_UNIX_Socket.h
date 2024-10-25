/* A class to facilitate internal UNIX socket communication
 * as controlling LEDs using ws281x lib needs direct access
 * to the RPi's hardware, which requires administrative privileges.
 *
 * Socket communication is created between two program of different
 * user level so the LED can be controlled by non-sudo program.
 *
 * Created by Tong Tin Yi (2023)
 */
/*******************************************************/
/* This is a stream socket client program for UNIX     */
/* domain sockets. It creates a socket, connects       */
/* to a server, sends data, then receives a returning  */
/* message from the server.                            */
/*******************************************************/
/**************************************/
/* all functions return int value for */
/* error handling. value 0 means okay */
/* if value is -1, error occurs       */
/**************************************/

#ifndef LED_UNIX_SOCKET_H
#define LED_UNIX_SOCKET_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>

#define SERVER_PATH "/tmp/tpf_unix_sock.server"
#define CLIENT_PATH "/tmp/tpf_unix_sock.client"

/**
* @brief A socket class to facilitate internal UNIX communication of LEDs
*/
class LED_UNIX_Socket
{ 
    public:
        LED_UNIX_Socket();
        virtual ~LED_UNIX_Socket();

        /**
         * @brief lightMode: the current mode of LEDs light
         */
        static int lightMode;
        /**
         * @brief LEDs shows white cycle running
         * @return -1, if error occurs
         */
        int WhiteCycle();
        /**
         * @brief LEDs show raibow color running in circle
         * @return -1, if error occurs
         */
        int RainbowCycle();
        /**
         * @brief LEDs show cycle of green light running
         * @return -1, if error occurs
         */
        int GreenCycle();
        /**
         * @brief All LEDs emit low intensity white light
         * @return -1, if error occurs
         */
        int SoftWhite();
        /**
         * @brief All LEDs emit medium intensity white light
         * @return -1, if error occurs
         */
        int MediumWhite();
        /**
         * @brief All LEDs emit high intensity white light
         * @return -1, if error occurs
         */
        int FullWhite();
        /**
         * @brief All LEDs turn off
         * @return -1, if error occurs
         */
        int Pause();
        /**
         * @brief LED socket closes
         * @return -1, if error occurs
         */
        int Stop();

    protected:


    private:
        int client_sock, rc, len;
        struct sockaddr_un server_sockaddr;
        struct sockaddr_un client_sockaddr;
        char buf[256];

        int status = 0;

        /**
         * @brief Sends command to LED_UNIX_Server
         * @param LED_CMD is the char array of command
         * @return -1, if error occurs
         */
        int sendCMD(char LED_CMD[256]);
        /**
         * @brief Reads message from LED_UNIX_Server
         * @return -1, if error occurs
         */
        int readReply();
};

#endif // LED_UNIX_SOCKET_H
