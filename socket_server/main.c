#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define PROTOPORT 8000
#define QLEN 6
#define BUFFERSIZE 512

char* copyString(char s[])
{
    char* s2;
    s2 = (char*)malloc(20);
    strcpy(s2, s);
    return (char*)s2;
}

void ErrorHandler(char *errorMessage) {
    printf ("%s", errorMessage);
}

void ClearWinSock() {
    #if defined WIN32
        WSACleanup();
    #endif
}

int main(int argc, char *argv[]) {
    // Start server on PROTOPORT or port passed by argument
    printf("Starting Server...\n");
    int port;
    port = PROTOPORT;
    if (argc > 1) {
        port = atoi(argv[1]);
    }

    if (port < 0) {
        printf("bad port number %s \n", argv[1]);
        return 0;
    }

    #if defined WIN32 // initialize Winsock
        WSADATA wsaData;
        int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (iResult != 0) {
            ErrorHandler("Error at WSAStartup()\n");
            return 0;
        }
    #endif

    // Initialize Socket
    int MySocket;
    MySocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (MySocket < 0) {
        ErrorHandler("socket creation failed.\n");
        ClearWinSock();
        return -1;
    }

    struct sockaddr_in sad;
    memset(&sad, 0, sizeof(sad));
    sad.sin_addr.s_addr = inet_addr("127.0.0.1");
    sad.sin_port = htons(port);
    if (bind(MySocket, (struct sockaddr*) &sad, sizeof(sad)) < 0) {
        ErrorHandler("bind() failed.\n");
        closesocket(MySocket);
        ClearWinSock();
        return -1;
    }

    if (listen (MySocket, QLEN) < 0) {
        ErrorHandler("listen() failed.\n");
        closesocket(MySocket);
        ClearWinSock();
        return -1;
    }

    // accept new connections
    struct sockaddr_in cad;
    int clientSocket;
    int clientLen;
    printf("Waiting for a client to connect...\n");

    while (1) {
        // check for incoming connections
        clientLen = sizeof(cad);
        clientSocket = accept(MySocket, (struct sockaddr *)&cad, &clientLen);
        if (clientSocket < 0) {
            ErrorHandler("accept() failed.\n");
            closesocket(MySocket);
            ClearWinSock();
            return 0;
        }
        printf("Handling client %s\n", inet_ntoa(cad.sin_addr));

        char success_msg[] = "connessione avvenuta correttamente";
        int success_msg_len = strlen(success_msg);
        if (write(clientSocket , success_msg , success_msg_len) != success_msg_len) {
            ErrorHandler("send() sent a different number of bytes than expected");
        }

        while (1) {
            int bytesRcvd;
            int totalBytesRcvd = 0;
            char buf[BUFFERSIZE];

            // listen A string from client
            bytesRcvd = recv(clientSocket, buf, BUFFERSIZE - 1, 0);
            if (bytesRcvd <= 0) {
                ErrorHandler("recv() failed or connection closed prematurely\n");
                break;
            }
            totalBytesRcvd += bytesRcvd;
            buf[bytesRcvd] = '\0';
            printf("Received: %s\n", buf);

            char* stringA;
            stringA = copyString(buf);

            // listen B string from client
            bytesRcvd = recv(clientSocket, buf, BUFFERSIZE - 1, 0);
            if (bytesRcvd <= 0) {
                ErrorHandler("recv() failed or connection closed prematurely\n");
                break;
            }
            totalBytesRcvd += bytesRcvd;
            buf[bytesRcvd] = '\0';
            printf("Received: %s\n", buf);

            char* stringB;
            stringB = copyString(buf);

            char* bye_message = "bye";
            char* quit_message = "quit";
            if (strcmp(stringA, quit_message) == 0 || strcmp(stringB, quit_message) == 0) {
                if (write(clientSocket , bye_message , strlen(bye_message)) != strlen(bye_message)) {
                    ErrorHandler("send() sent a different number of bytes than expected\n");
                }
                break;
            }

            strcat(stringA, stringB);
            char* stringC = stringA;
            int stringC_len = strlen(stringC);
            if (write(clientSocket , stringC , stringC_len) != stringC_len) {
                ErrorHandler("send() sent a different number of bytes than expected\n");
            }
        }

    }
}


