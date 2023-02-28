// wireless.cpp

// Includes
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include <ti/display/Display.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/net/wifi/device.h>
#include <ti/drivers/net/wifi/simplelink.h>
#include <ti/drivers/net/wifi/wlan.h>

#include "ti_drivers_config.h"
#include "wireless.hh"
// end Includes

// Global vars
pthread_t slSpawnThread = (pthread_t)NULL;
// end Global vars

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
    uint32_t            RetVal;
    pthread_attr_t      pAttrs_spawn;
    struct sched_param  priParam;
    int16_t start_retval = 0;
    int16_t stop_retval = 0;

    // Create sl_Task
    pthread_attr_init(&pAttrs_spawn);
    priParam.sched_priority = SPAWN_TASK_PRIORITY;
    RetVal = pthread_attr_setschedparam(&pAttrs_spawn, &priParam);
    RetVal |= pthread_attr_setstacksize(&pAttrs_spawn, TASK_STACK_SIZE);
    RetVal = pthread_create(&slSpawnThread, &pAttrs_spawn, sl_Task, NULL);

    if (RetVal) {
        printf("Spawn task error\n");
        while(1) {}
    } else {
        printf("Task spawned\n");
    }

    printf("0, start_retval: %d (0x%x)\n", start_retval, start_retval);
    //printf("0, stop_retval:  %d (0x%x)\n", stop_retval, stop_retval);

    printf("sl_Start\n");
    //sl_WlanSetMode(ROLE_AP);
    sl_Stop(SL_STOP_TIMEOUT);

    GPIO_write(10, 1);   // Amber

    start_retval = sl_Start(NULL, NULL, NULL);

    GPIO_write(10, 0);   // Amber
    GPIO_write(11, 1);   // Green

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

    GPIO_write(11, 0);  // Green

    stop_retval = sl_Stop(SL_STOP_TIMEOUT);

    GPIO_write(9, 1);   // Red
    printf("2, start_retval: %d (0x%x)\n", start_retval, start_retval);
    printf("2, stop_retval:  %d (0x%x)\n", stop_retval, stop_retval);

    printf("Wireless module finished!\n");

    return;
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
