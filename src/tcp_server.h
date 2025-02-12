#ifndef PICOW_AS_REMOTE_DEBUGGER_TCP_SERVER_H
#define PICOW_AS_REMOTE_DEBUGGER_TCP_SERVER_H

#include "FreeRTOS.h"
#include "event_groups.h"
#include "ipstack/IPStack.h"
#include "lwip/api.h"

#define BIT_0 (1 << 0)

void tcp_server_task(void *pvParameters);



#endif //PICOW_AS_REMOTE_DEBUGGER_TCP_SERVER_H
