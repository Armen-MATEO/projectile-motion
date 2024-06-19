

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#define PI 3.14159265

int main(int argc, char *argv[])
{

    SOCKET serverSocket, acceptSocket;
    int PORT = 55555;
    WSADATA wsaData;
    int wsaError;
    WORD winVersionRequested = MAKEWORD(2, 2);
    wsaError = WSAStartup(winVersionRequested, &wsaData);
    if (wsaError != 0)
    {
        printf("the winsock ddl has not found");
        return 0;
    }
    else
    {
        printf("winsock ddl has found\n");
        printf("the status %s \n", wsaData.szSystemStatus);
    }

    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET)
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
    if (bind(serverSocket, (SOCKADDR *)&service, sizeof(service)) == SOCKET_ERROR)
    {
        printf("bind function failed %s \n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
    else
    {
        printf("bind function is ok \n");
    }

    if (listen(serverSocket, 1) == SOCKET_ERROR)
    {
        printf("error in listening \n %s", WSAGetLastError());
    }

    else
    {
        printf("it is waiting for connection \n");
    }

    acceptSocket = accept(serverSocket, NULL, NULL);
    if (acceptSocket == INVALID_SOCKET)
    {
        printf("accept function failed \n %s", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    printf("connection accepted \n");

    char velocitybuffer[200];

    int velocitybyte = recv(acceptSocket, velocitybuffer, 200, 0);
    if (velocitybyte > 0)
    {
        printf("velocity has been received \n %s", velocitybuffer);
    }
    else
    {
        WSACleanup();
    }

    char anglebuffer[200];

    int anglebyte = recv(acceptSocket, anglebuffer, 200, 0);
    if (anglebyte > 0)
    {
        printf("angle has been received \n %s", anglebuffer);
    }
    else
    {
        WSACleanup();
    }

    char getorpostbuffer[200];

    int getpostbyte = recv(acceptSocket, getorpostbuffer, 200, 0);
    printf("getpostbyte \n %s", getorpostbuffer);
    if (getpostbyte > 0)
    {

        printf("get or post has been received \n %s", getorpostbuffer);
        int getorpost = atoi(getorpostbuffer);

        if (getorpost == 1)
        {

            double rad, g, Vx, Vy, t;
            double vo = atof(velocitybuffer);
            double ang = atof(anglebuffer);

            g = 9.81;
            rad = ang * PI / 180.0;
            t = (2.0 * vo * sin(rad) / g);
            Vy = (vo * vo * sin(rad) * sin(rad)) / (2.0 * g);
            Vx = (vo * vo * sin(2.0 * rad)) / g;

            char arr[200];
            sprintf(arr, " %f", t);
            char maxHeight[200];
            sprintf(maxHeight, " %f", Vy);

            int byteCount = send(acceptSocket, arr, 200, 0);
            if (byteCount > 0)
            {
                printf("flight has been sent   %f  \n", t);
            }
            else
            {
                WSACleanup();
            }
            int heightbyte = send(acceptSocket, maxHeight, 200, 0);
            if (heightbyte > 0)
            {
                printf("height has been sent  %f \n", Vy);
            }
            else
            {
                WSACleanup();
            }
        }
        if (getorpost == 2)
        {
            FILE *fptr;

            fptr = fopen("velocityangle.txt", "a");

            if (fptr != NULL)
            {
                double rad, g, Vx, Vy, t;
                double vo = atof(velocitybuffer);
                double ang = atof(anglebuffer);

                g = 9.81;
                rad = ang * PI / 180.0;
                t = (2.0 * vo * sin(rad) / g);
                Vy = (vo * vo * sin(rad) * sin(rad)) / (2.0 * g);
                Vx = (vo * vo * sin(2.0 * rad)) / g;

                char arr[200];
                sprintf(arr, " %f", t);
                char maxHeight[200];
                sprintf(maxHeight, " %f", Vy);

                fprintf(fptr, "\n velocity", "a");
                fprintf(fptr, velocitybuffer, "a");
                fprintf(fptr, "\n angle", "a");
                fprintf(fptr, anglebuffer, "a");
                fprintf(fptr, "\n maximum height", "a");
                fprintf(fptr, maxHeight, "a");
                fprintf(fptr, "\n time of flight", "a");
                fprintf(fptr, arr, "a");

                char saved[200] = " saved successfuly";
                int savedbyte = send(acceptSocket, saved, 200, 0);
                if (savedbyte > 0)
                {
                    printf("post saved, success ");
                }
                else
                {
                    WSACleanup();
                }

                fclose(fptr);
            }
        }
    }

    else
    {
        WSACleanup();
    }

    system("pause");
    WSACleanup();

    return 0;
}