#include "tcp_server.h"

void tcp_server_task(void *pvParameters) {
    auto init_complete_event = (EventGroupHandle_t*) pvParameters;
    xEventGroupWaitBits(*init_complete_event, BIT_0, pdFALSE, pdFALSE, portMAX_DELAY);

    printf("Starting TCP Server...\n");

    struct netconn *server_conn, *client_conn;
    struct netbuf *buf;
    void *data;
    u16_t len;
    err_t err;

    // Create a new TCP connection
    server_conn = netconn_new(NETCONN_TCP);
    if (!server_conn) {
        printf("Error: Unable to create netconn!\n");
        vTaskDelete(NULL);
        return;
    }

    // Bind the server to the Pico W's IP address and port
    err = netconn_bind(server_conn, IP_ADDR_ANY, 50372);
    if (err != ERR_OK) {
        printf("Error: netconn_bind failed with code %d\n", err);
        netconn_delete(server_conn);
        vTaskDelete(nullptr);
        return;
    }

    // Start listening for incoming connections
    netconn_listen(server_conn);
    printf("TCP Server listening on port 50372...\n");

    while (true) {
        // Accept an incoming connection
        err = netconn_accept(server_conn, &client_conn);
        if (err == ERR_OK) {
            printf("Client connected!\n");

            // Receive data from the client
            while ((err = netconn_recv(client_conn, &buf)) == ERR_OK) {
                netbuf_data(buf, &data, &len);
                printf("Received: %.*s\n", len, (char*)data);

                // Send a response
                const char *response = "Hello, client!";
                netconn_write(client_conn, response, strlen(response), NETCONN_COPY);

                netbuf_delete(buf); // Free the buffer
            }

            // Close the connection
            netconn_close(client_conn);
            netconn_delete(client_conn);
            printf("Client disconnected.\n");
        } else {
            printf("netconn_accept failed with error %d\n", err);
        }
    }
}
