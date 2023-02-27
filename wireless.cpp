// wireless.cpp

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include <ti/display/Display.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/net/wifi/device.h>
#include <ti/drivers/net/wifi/simplelink.h>
#include <ti/drivers/net/wifi/wlan.h>

#include "ti_drivers_config.h"
#include "wireless.hh"

#define SL_STOP_TIMEOUT 200

void SimpleLinkInitCallback(uint32_t status, SlDeviceInitInfo_t *DeviceInitInfo);

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
    /*
    int16_t start_retval = 0;
    int16_t stop_retval = 0;

    printf("0, start_retval: %d (0x%x)\n", start_retval, start_retval);
    printf("0, stop_retval:  %d (0x%x)\n", stop_retval, stop_retval);

    printf("sl_Start\n");
    printf("sl_Start");

    sl_WlanSetMode(ROLE_AP);
    sl_Stop(SL_STOP_TIMEOUT);
    start_retval = sl_Start(NULL, NULL, NULL);

    printf("1, start_retval: %d (0x%x)\n", start_retval, start_retval);
    printf("1, stop_retval:  %d (0x%x)\n", stop_retval, stop_retval);

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

    stop_retval = sl_Stop(SL_STOP_TIMEOUT);

    printf("2, start_retval: %d (0x%x)\n", start_retval, start_retval);
    printf("2, stop_retval:  %d (0x%x)\n", stop_retval, stop_retval);

    printf("Wireless module finished!\n");

    return;
    */

    int32_t            iRetVal = 0;

    iRetVal = sl_Start(NULL, NULL, (P_INIT_CALLBACK)SimpleLinkInitCallback);
    /* Initialize Simple Link */
    if(iRetVal < 0) {
        printf("sl_Start Failed\r\n");
        if (iRetVal == SL_ERROR_RESTORE_IMAGE_COMPLETE) {
            printf(
                "\r\n**********************************\r\n"
                "Return to Factory Default been Completed\r\n"
                "Please RESET the Board\r\n"
                "**********************************\r\n");
        }
        while(1) {}
    }

    else {
        printf("sl_Start returned.\n");
        while(1) {}
    }


}

void SimpleLinkInitCallback(uint32_t status, SlDeviceInitInfo_t *DeviceInitInfo) {
    if ((int32_t)status == SL_ERROR_RESTORE_IMAGE_COMPLETE) {
        printf(
            "\r\n**********************************\r\nReturn to"
            " Factory Default been Completed\r\nPlease RESET the Board\r\n"
            "**********************************\r\n");

        while(1) {}
    }

    printf("status: %d\n", status);


    printf("Device started in %s role\n\r", (0 == status) ? "Station" :\
        (2 == status) ? "AP" : ((3 == status) ? "P2P" : "Start Role error"));

    if (ROLE_STA == status) {
        printf("ROLE_STA\n");
        printf("AppEvent_STARTED");
    } else if (ROLE_AP == status) {
        printf("ROLE_AP\n");
        printf("AppEvent_RESTART");
    } else {
        printf("AppEvent_ERROR");
    }
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
