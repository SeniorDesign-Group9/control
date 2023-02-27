// wireless.cpp

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include "ti_drivers_config.h"
#include <ti/drivers/SPI.h>
#include <ti/drivers/net/wifi/device.h>
#include <ti/drivers/net/wifi/simplelink.h>
#include <ti/drivers/net/wifi/wlan.h>
#include "wireless.hh"

// Singleton functions
// Empty constructor
Wireless::Wireless() {}

// Empty destructor
Wireless::~Wireless() {}

// Instance
Wireless& Wireless::instance() {
    static Wireless inst;
    return inst;
}

// Class functions
void Wireless::init() {

    int16_t start_retval = 0;
    int16_t stop_retval = 0;

    printf("0, start_retval: 0x%x\n", start_retval);
    printf("0, stop_retval: 0x%x\n", stop_retval);

    printf("sl_Start\n");
    start_retval = sl_Start(0, 0, 0);

    printf("1, start_retval: 0x%x\n", start_retval);
    printf("1, stop_retval: 0x%x\n", stop_retval);

    switch (start_retval) {
        case ROLE_STA:
            printf("ROLE_STA\n");
            break;

        case ROLE_RESERVED:
            printf("ROLE_RESERVED\n");
            break;

        case ROLE_AP:
            printf("ROLE_AP\n");
            break;

        case ROLE_P2P:
            printf("ROLE_P2P\n");
            break;

        case ROLE_TAG:
            printf("ROLE_TAG\n");
            break;

        case -1:
            printf("error\n");
            break;

        default:
            printf("default\n");
            break;
    }

    sleep(1);

    printf("sl_Stop\n");
    stop_retval = sl_Stop(0xFFFF);

    printf("2, start_retval: 0x%x\n", start_retval);
    printf("2, stop_retval: 0x%x\n", stop_retval);

    printf("Wireless module finished!\n");

}

// Event handlers
void SimpleLinkHttpServerEventHandler(SlNetAppHttpServerEvent_t *pSlHttpServerEvent, SlNetAppHttpServerResponse_t *pSlHttpServerResponse) {}
void SimpleLinkNetAppRequestMemFreeEventHandler(uint8_t *buffer) {}
void SimpleLinkWlanEventHandler(SlWlanEvent_t *pSlWlanEvent) {}
void SimpleLinkFatalErrorEventHandler(SlDeviceFatal_t *slFatalErrorEvent) {
    switch (slFatalErrorEvent->Id) {
        case SL_DEVICE_EVENT_FATAL_DEVICE_ABORT:
            printf("[ERROR] - FATAL ERROR: Abort NWP event detected: AbortType=%d, AbortData=0x%x", slFatalErrorEvent->Data.DeviceAssert.Code, slFatalErrorEvent->Data.DeviceAssert.Value);
            break;

        case SL_DEVICE_EVENT_FATAL_DRIVER_ABORT:
            printf("[ERROR] - FATAL ERROR: Driver Abort detected. ");
            break;

        case SL_DEVICE_EVENT_FATAL_NO_CMD_ACK:
            printf("[ERROR] - FATAL ERROR: No Cmd Ack detected [cmd opcode = 0x%x] ", slFatalErrorEvent->Data.NoCmdAck.Code);
            break;

        case SL_DEVICE_EVENT_FATAL_SYNC_LOSS:
            printf("[ERROR] - FATAL ERROR: Sync loss detected n\r");
            break;

        case SL_DEVICE_EVENT_FATAL_CMD_TIMEOUT:
            printf("[ERROR] - FATAL ERROR: Async event timeout detected [event opcode =0x%x]  ", slFatalErrorEvent->Data.CmdTimeout.Code);
            break;

        default:
            printf("[ERROR] - FATAL ERROR: Unspecified error detected ");
            break;
    }
}
void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent) {}
void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent) {
    printf("[GENERAL EVENT] - ID=[%d] Sender=[%d]\n\n", pDevEvent->Data.Error.Code, pDevEvent->Data.Error.Source);
}
void SimpleLinkSockEventHandler(SlSockEvent_t *pSock) {}
void SimpleLinkNetAppRequestEventHandler(SlNetAppRequest_t *pNetAppRequest, SlNetAppResponse_t *pNetAppResponse) {}

