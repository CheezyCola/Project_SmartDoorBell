#include "LED_UNIX_Socket.h"

int LED_UNIX_Socket::lightMode = 0;

LED_UNIX_Socket::LED_UNIX_Socket()
{
    // clear memory
    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));

    /**************************************/
    /* Create a UNIX domain stream socket */
    /**************************************/
    client_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_sock == -1) {
        perror("CLIENT SOCKET ERROR");
        status = -1;
    }

    #ifdef __linux__
    fchmod(client_sock, S_IRUSR | S_IWUSR);
    #endif // __linux__
    /***************************************/
    /* Set up the UNIX sockaddr structure  */
    /* by using AF_UNIX for the family and */
    /* giving it a filepath to bind to.    */
    /*                                     */
    /* Unlink the file so the bind will    */
    /* succeed, then bind to that file.    */
    /***************************************/
    client_sockaddr.sun_family = AF_UNIX;
    strcpy(client_sockaddr.sun_path, CLIENT_PATH);
    len = sizeof(client_sockaddr);

    unlink(CLIENT_PATH);
    rc = bind(client_sock, (struct sockaddr *) &client_sockaddr, len);
    if (rc == -1){
        perror("CLIENT BIND ERROR");
        close(client_sock);
        status = -1;
    }

    /***************************************/
    /* Set up the UNIX sockaddr structure  */
    /* for the server socket and connect   */
    /* to it.                              */
    /***************************************/
    server_sockaddr.sun_family = AF_UNIX;
    strcpy(server_sockaddr.sun_path, SERVER_PATH);
    rc = connect(client_sock, (struct sockaddr *) &server_sockaddr, len);
    if(rc == -1){
        perror("CLIENT CONNECT ERROR");
        close(client_sock);
        status = -1;
    }
}

LED_UNIX_Socket::~LED_UNIX_Socket()
{
    close(client_sock);
    unlink(CLIENT_PATH);
}

int LED_UNIX_Socket::sendCMD(char LED_CMD[256])
{
    if (status == -1) return status;
    /************************************/
    /* Copy the data to the buffer and  */
    /* send it to the server socket.    */
    /************************************/
    strcpy(buf, LED_CMD);
    rc = send(client_sock, buf, strlen(buf), 0);
    if (rc == -1) {
        perror("CLIENT SEND ERROR");
        return rc;
    }
    return status;
}

int LED_UNIX_Socket::readReply()
{
    if (status == -1) return status;
    /**************************************/
    /* Read the data sent from the server */
    /**************************************/
    memset(buf, 0, sizeof(buf));    // clear buffer
    rc = read(client_sock, buf, sizeof(buf));
    if (rc == -1) {
        perror("CLIENT RECEIVE ERROR");
        return rc;
    }
    printf("LED Server responded to Command \" %s \"\n", buf);
    close(client_sock);
    unlink(CLIENT_PATH);

    return status;
}

int LED_UNIX_Socket::WhiteCycle()
{
    char myString[] = "White Cycle";
    if(sendCMD(myString) == -1)
        return status = -1;
    else
        lightMode = 6;

    if (readReply() == -1)
        return status = -1;

    return status;
}

int LED_UNIX_Socket::RainbowCycle()
{
    char myString[] = "Rainbow Cycle";
    if(sendCMD(myString) == -1)
        return status = -1;
    else
        lightMode = 5;

    if (readReply() == -1)
        return status = -1;

    return status;
}

int LED_UNIX_Socket::GreenCycle()
{
    char myString[] = "Green Cycle";
    if(sendCMD(myString) == -1)
        return status = -1;
    else
        lightMode = 4;

    if (readReply() == -1)
        return status = -1;

    return status;
}

int LED_UNIX_Socket::SoftWhite()
{
    char myString[] = "Soft White";
    if(sendCMD(myString) == -1)
        return status = -1;
    else
        lightMode = 3;

    if (readReply() == -1)
        return status = -1;

    return status;
}

int LED_UNIX_Socket::MediumWhite()
{
    char myString[] = "Medium White";
    if(sendCMD(myString) == -1)
        return status = -1;
    else
        lightMode = 2;

    if (readReply() == -1)
        return status = -1;

    return status;
}

int LED_UNIX_Socket::FullWhite()
{
    char myString[] = "Full White";
    if(sendCMD(myString) == -1)
        return status = -1;
    else
        lightMode = 1;

    if (readReply() == -1)
        return status = -1;

    return status;
}

int LED_UNIX_Socket::Pause()
{
    char myString[] = "Pause";
    if(sendCMD(myString) == -1)
        return status = -1;
    else
        lightMode = 0;

    if (readReply() == -1)
        return status = -1;

    return status;
}

int LED_UNIX_Socket::Stop()
{
    char myString[] = "Stop";
    if(sendCMD(myString) == -1)
        return status = -1;

    if (readReply() == -1)
        return status = -1;

    return status;
}
