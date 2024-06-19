
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{

    SOCKET clientSocket;
    int PORT = 55555;
    WSADATA wsaData;
    int wsaError;
    WORD winVersionRequested = MAKEWORD(2, 2);
    wsaError = WSAStartup(winVersionRequested, &wsaData);
    if (wsaError != 0)
    {
        printf("the winsock ddl not found");
        return 0;
    }
    else
    {
        printf("winsock ddl has found\n");
        printf("the status %s \n", wsaData.szSystemStatus);
    }

    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET)
    {
        printf("error at function socket %s \n ", WSAGetLastError());
        WSACleanup();
        return 0;
    }
    else
    {
        printf("socket function is ok \n");
    }

    SOCKADDR_IN service;
    service.sin_family = AF_INET;
    InetPton(AF_INET, "127.0.0.1", &service.sin_addr.s_addr);
    service.sin_port = htons(PORT);

    if (connect(clientSocket, (SOCKADDR *)&service, sizeof(service)) == SOCKET_ERROR)
    {
        printf("connect function has failed %s \n", WSAGetLastError());

        WSACleanup();
        return 0;
    }
    else
    {
        printf("the connect function is ok \n");
    }

    char veocitybuffer[200];

    printf("please enter the initial velocity \n");

    fgets(veocitybuffer, 200, stdin);
    int velocitybyte = send(clientSocket, veocitybuffer, 200, 0);
    if (velocitybyte > 0)
    {
        printf("initial velocity has been sent \n %s", veocitybuffer);
    }
    else
    {
        WSACleanup();
    }

    char anglebuffer[200];

    printf("please enter the launche angle \n");

    fgets(anglebuffer, 200, stdin);
    int anglebyte = send(clientSocket, anglebuffer, 200, 0);
    if (anglebyte > 0)
    {
        printf("launche angle has been sent \n %s", anglebuffer);
    }
    else
    {
        WSACleanup();
    }

    char getorpostbuffer[200];

    printf("if you want get, choose 1, if post, choose 2 \n 1 get\n 2 post\n");

    fgets(getorpostbuffer, 200, stdin);
    int getpstbyte = send(clientSocket, getorpostbuffer, 200, 0);
    int getorpost = atoi(getorpostbuffer);

    if (getpstbyte > 0 && getorpost == 1)
    {
        printf("get or post has been sent \n %s", getorpostbuffer);

        char answerbuffer[200];
        int answerbyte = recv(clientSocket, answerbuffer, 200, 0);

        double answer = atof(answerbuffer);
        if (answerbyte > 0)
        {
            printf("the time of flight is  %f  \n", answer);
        }
        else
        {
            WSACleanup();
        }

        char heightbuffer[200];
        int heightbyte = recv(clientSocket, heightbuffer, 200, 0);

        double answer2 = atof(heightbuffer);
        if (heightbyte > 0)
        {
            printf("the maximum height is  %f  \n", answer2);
        }
        else
        {
            WSACleanup();
        }
        Sleep(1);
    }
    if (getpstbyte > 0 && getorpost == 2)
    {

        char savedbuffer[200];
        int savedbyte = recv(clientSocket, savedbuffer, 200, 0);

        if (savedbyte > 0)
        {
            printf("the post has been saved  %s  \n", savedbuffer);
        }
        else
        {
            WSACleanup();
        }
    }
    else
    {
        WSACleanup();
    }

    system("pause ");
    WSACleanup();

    return 0;
}