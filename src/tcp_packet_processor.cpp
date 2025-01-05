/*
    Callback Function for Incoming TCP Packets
    Plan:
    - Unpack the TCP/IP packet and trigger either:
        - The cdc_task function in the debug probe firmware, or
        - The CMSIS-DAP command handler.
    Suggestions:
    - Define clear packet format expectations.
    - Use a queue to safely pass data to the cdc_task (or CMSIS-DAP handler) running in another task.
*/

#include "tcp_packet_processor.h"