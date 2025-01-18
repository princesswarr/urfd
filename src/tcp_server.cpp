/*
    RTOS Task for TCP Socket Listening
    Plan:
    - Create a TCP socket that actively listens on a port.
    Suggestions:
    - Port Number: Debug-related services often use ports in the 49152-65535 range (dynamic/private ports)
    - Open a TCP socket on the chosen port.
    - Bind it to the Pico W’s IP address.
    - Listen for incoming connections.
*/

// below is AI gernated code used to test that libraries are being linked correctly

#include "FreeRTOS.h"
#include "task.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"

#define TCP_SERVER_PORT 50000  // Choose a port in the private/dynamic range

void tcp_server_task(void *pvParameters) {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    __socklen_t client_len = sizeof(client_addr);

    // 1. Create a TCP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        printf("Error: Failed to create socket\n");
        vTaskDelete(NULL);  // Exit task
    }

    // 2. Bind the socket to the chosen port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // Bind to all interfaces
    server_addr.sin_port = htons(TCP_SERVER_PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Error: Failed to bind socket\n");
        close(server_socket);
        vTaskDelete(NULL);  // Exit task
    }

    // 3. Start listening for incoming connections
    if (listen(server_socket, 1) < 0) {  // Allow 1 queued connection
        printf("Error: Failed to listen on socket\n");
        close(server_socket);
        vTaskDelete(NULL);  // Exit task
    }
    printf("TCP server listening on port %d\n", TCP_SERVER_PORT);

    // 4. Accept incoming connections
    while (1) {
        printf("Waiting for a client to connect...\n");
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            printf("Error: Failed to accept connection\n");
            continue;  // Keep listening
        }

        printf("Client connected from IP: %s, Port: %d\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));

        // Handle the client (e.g., read/write data)
        char buffer[128];
        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';  // Null-terminate the buffer
            printf("Received: %s\n", buffer);
        }

        // Close the client connection
        close(client_socket);
        printf("Client disconnected\n");
    }

    // Close the server socket (will not reach here in this infinite loop)
    close(server_socket);
    vTaskDelete(NULL);  // Exit task
}
