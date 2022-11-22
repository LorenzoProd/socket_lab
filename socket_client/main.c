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
#define BUFFERSIZE 512

void ErrorHandler(char *errorMessage) {
    printf("%s",errorMessage);
}

void ClearWinSock() {
#if defined WIN32
    WSACleanup();
#endif
}

int main(void) {
    #if defined WIN32
        WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2 ,2), &wsaData);
    if (iResult != 0) {
        printf ("error at WSASturtup\n");
        return -1;
    }
    #endif

    // Initialize Socket
    int Csocket;
    Csocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (Csocket < 0) {
        ErrorHandler("socket creation failed.\n");
        closesocket(Csocket);
        ClearWinSock();
        return -1;
    }

    // read port
    int port;
    printf("Enter a port number: ");
    scanf("%d", &port);

    struct sockaddr_in sad;
    memset(&sad, 0, sizeof(sad));
    sad.sin_family = AF_INET;
    sad.sin_addr.s_addr = inet_addr("127.0.0.1");
    sad.sin_port = htons(port);

    // connection to server
    if (connect(Csocket, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
        ErrorHandler( "Failed to connect.\n" );
        closesocket(Csocket);
        ClearWinSock();
        return -1;
    }

    // receive connection success message
    int bytesRcvd;
    int totalBytesRcvd = 0;
    char buf[BUFFERSIZE];
    printf("");

    if ((bytesRcvd = recv(Csocket, buf, BUFFERSIZE - 1, 0)) <= 0) {
        ErrorHandler("recv() failed or connection closed prematurely");
        closesocket(Csocket);
        ClearWinSock();
        return -1;
    }
    totalBytesRcvd += bytesRcvd;
    buf[bytesRcvd] = '\0';
    printf("Received: %s\n", buf);

    while (1) {
        // send string A to server
        char stringA[BUFFERSIZE] = "";
        printf("Enter A string: ");
        scanf("%s", stringA);
        int stringA_len = strlen(stringA);
        if (write(Csocket, stringA, stringA_len) != stringA_len) {
            ErrorHandler("send() sent a different number of bytes than expected");
            closesocket(Csocket);
            ClearWinSock();
            return -1;
        }

        // send string B to server
        char stringB[BUFFERSIZE] = "";
        printf("Enter B string: ");
        scanf("%s", stringB);
        int stringB_len = strlen(stringB);
        if (send(Csocket, stringB, stringB_len, 0) != stringB_len) {
            ErrorHandler("send() sent a different number of bytes than expected");
            closesocket(Csocket);
            ClearWinSock();
            return -1;
        }

        // receive concat A + B string
        if ((bytesRcvd = recv(Csocket, buf, BUFFERSIZE - 1, 0)) <= 0) {
            ErrorHandler("recv() failed or connection closed prematurely");
            closesocket(Csocket);
            ClearWinSock();
            return -1;
        }
        totalBytesRcvd += bytesRcvd;
        buf[bytesRcvd] = '\0';

        // close connection if server sent 'bye' message or print A + B string
        if (strcmp(buf, "bye") == 0) {
            closesocket(Csocket);
            ClearWinSock();
            break;
        } else
            printf("A + B: %s\n\n", buf);



    }
    return(0);

}