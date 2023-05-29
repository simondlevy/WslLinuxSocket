// https://devblogs.microsoft.com/commandline/windowswsl-interop-with-af_unix/

#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define RECVSIZE size_t
#undef TEXT

#pragma comment(lib,"ws2_32.lib")

#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <afunix.h>
#include <stdlib.h>
#include <stdio.h>
#include <ws2tcpip.h>

#define SERVER_SOCKET "server.sock"

static void error(
        const SOCKET ClientSocket,
        const SOCKET ListenSocket,
        const char * cmd,
        const int result)
{
    printf("%s failed with error: %d\n", cmd, result);

    // cleanup
    if (ListenSocket != INVALID_SOCKET) {
        closesocket(ListenSocket);
    }

    if (ClientSocket != INVALID_SOCKET) {
        closesocket(ClientSocket);
    }

    // Analogous to `unlink`
    DeleteFileA(SERVER_SOCKET);
    WSACleanup();

    exit(1);
}

static void error(
        const SOCKET ClientSocket,
        const SOCKET ListenSocket,
        const char * cmd)
{
    error(ClientSocket, ListenSocket, cmd, WSAGetLastError());
}

int main()
{
    SOCKET ClientSocket = INVALID_SOCKET;
    SOCKET ListenSocket = INVALID_SOCKET;
    int Result = 0;
    char SendBuffer[] = "af_unix from Windows to WSL!";
    int SendResult = 0;
    SOCKADDR_UN ServerSocket = { 0 };
    WSADATA WsaData = { 0 };

    // Initialize Winsock
    Result = WSAStartup(MAKEWORD(2, 2), &WsaData);
    if (Result != 0) {
        error(ClientSocket, ListenSocket, "WSAStartup", Result);
    }

    // Create a AF_UNIX stream server socket.
    ListenSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (ListenSocket == INVALID_SOCKET) {
        error(ClientSocket, ListenSocket, "socket");
    }

    ServerSocket.sun_family = AF_UNIX;
    strncpy_s(
            ServerSocket.sun_path,
            sizeof ServerSocket.sun_path,
            SERVER_SOCKET, (sizeof SERVER_SOCKET) - 1);

    // Bind the socket to the path.
    Result = bind(
            ListenSocket,
            (struct sockaddr*)&ServerSocket, sizeof(ServerSocket));
    if (Result == SOCKET_ERROR) {
        error(ClientSocket, ListenSocket, "bind");
    }

    // Listen to start accepting connections.
    Result = listen(ListenSocket, SOMAXCONN);
    if (Result == SOCKET_ERROR) {
        error(ClientSocket, ListenSocket, "listen");
    }

    printf("Accepting connections on: '%s'\n", SERVER_SOCKET);
    // Accept a connection.
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        error(ClientSocket, ListenSocket, "accept");
    }
    printf("Accepted a connection.\n");

    // Send some data.

    while (true) {

        SendResult = send(ClientSocket, SendBuffer, (int)strlen(SendBuffer), 0);
        if (SendResult == SOCKET_ERROR) {
            error(ClientSocket, ListenSocket, "sendn");
        }
        printf("Relayed %zu bytes: '%s'\n", strlen(SendBuffer), SendBuffer);
    }

    // shutdown the connection.
    printf("Shutting down\n");
    Result = shutdown(ClientSocket, 0);
    if (Result == SOCKET_ERROR) {
        error(ClientSocket, ListenSocket, "shutdown");
    }

    return 0;
}
