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
#include <iostream>
#include <cstring>
#include "FreeRTOS.h"
#include "task.h"
#include "hardware/gpio.h"
#include "ipstack/IPStack.h"

#include "hardware/timer.h"

#define TCP_SERVER_PORT 50000  // Choose a port in the private/dynamic range
#define HTTP_SERVER        "3.224.58.169"
#define BUFSIZE 2048
#define WIFI_SSID "franks_galaxy"
#define WIFI_PASSWORD "veef2267"

void tcp_server_task(void *pvParameters) {
    (void) pvParameters;
    printf("i am here2\n");
    auto *msg = "Hello, Frank!";
    printf("\nconnecting...\n");

    unsigned char *buffer = new unsigned char[BUFSIZE];
    // todo: Add failed connection handling
    //IPStack ipstack("SmartIotMQTT", "SmartIot"); // example
    IPStack ipstack(WIFI_SSID, WIFI_PASSWORD); // Set env in CLion CMAKE setting

    while(true) {
        int rc = ipstack.connect(HTTP_SERVER, 80);
        if (rc == 0) {
            ipstack.write((unsigned char *) (msg), strlen(msg), 1000);
            auto rv = ipstack.read(buffer, BUFSIZE, 2000);
            buffer[rv] = 0;
            printf("rv=%d\n%s\n", rv, buffer);
            ipstack.disconnect();
        }
        else {
            printf("rc from TCP connect is %d\n", rc);
        }
    }
}
