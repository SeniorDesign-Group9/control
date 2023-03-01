// wireless.cpp

// Includes
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/net/wifi/device.h>
#include <ti/drivers/net/wifi/fs.h>
#include <ti/drivers/net/wifi/netapp.h>
#include <ti/drivers/net/wifi/netcfg.h>
#include <ti/drivers/net/wifi/simplelink.h>
#include <ti/drivers/net/wifi/sl_socket.h>
#include <ti/drivers/net/wifi/trace.h>
#include <ti/drivers/net/wifi/wlan.h>

#include "ti_drivers_config.h"
#include "wireless.hh"
// end Includes


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
    int16_t start_retval = 0;
    int16_t stop_retval = 0;
    uint8_t pConfigOpt = 0;
    uint16_t pConfigLen = 0;
    int16_t status = 0;

    SlDeviceVersion_t ver;
    /*
    _i32 slRetVal;
    SlFsRetToFactoryCommand_t RetToFactoryCommand;
    _i32 Status, ExtendedStatus;
    RetToFactoryCommand.Operation = SL_FS_FACTORY_RET_TO_DEFAULT;
    Status = sl_FsCtl( (SlFsCtl_e)SL_FS_CTL_RESTORE, 0, NULL, (_u8 *)&RetToFactoryCommand, sizeof(SlFsRetToFactoryCommand_t), NULL, 0 , NULL );
    if ((_i32)Status < 0) {
        //error
        //Status is composed from Signed error number & extended status
        Status = (_i16)Status>> 16;
        ExtendedStatus = (_u16)slRetVal& 0xFFFF;
        return;
    }
    */
    //Reset
    // printf("sl_Stop:  %d\n", sl_Stop(SL_STOP_TIMEOUT));
    printf("sl_Start: %d\n", sl_Start(NULL, NULL, NULL));
    // printf("sl_Stop:  %d\n", sl_Stop(SL_STOP_TIMEOUT));
    /*
    //sl_WlanSetMode(ROLE_AP);
    sl_Stop(SL_STOP_TIMEOUT);
    start_retval = sl_Start(NULL, NULL, NULL);

    GPIO_write(11, 0);  // Green
    */


    pConfigLen = sizeof(ver);
    pConfigOpt = SL_DEVICE_GENERAL_VERSION;
    status = sl_DeviceGet(SL_DEVICE_GENERAL,&pConfigOpt,&pConfigLen,(_u8 *)(&ver));
    printf("status = %d\n", status);
     printf("CHIP 0x%x\nMAC 31.%d.%d.%d.%d\nPHY %d.%d.%d.%d\nNWP %d.%d.%d.%d\nROM %d\nHOST %d.%d.%d.%d\n",
                     ver.ChipId,
                     ver.FwVersion[0],ver.FwVersion[1],
                     ver.FwVersion[2],ver.FwVersion[3],
                     ver.PhyVersion[0],ver.PhyVersion[1],
                     ver.PhyVersion[2],ver.PhyVersion[3],
                     ver.NwpVersion[0],ver.NwpVersion[1],ver.NwpVersion[2],ver.NwpVersion[3],
                     ver.RomVersion,
                     SL_MAJOR_VERSION_NUM,SL_MINOR_VERSION_NUM,SL_VERSION_NUM,SL_SUB_VERSION_NUM);

     SlDateTime_t dateTime= {0};
     dateTime.tm_day =   (_u32)1;          // Day of month (DD format) range 1-31
     dateTime.tm_mon =   (_u32)3;           // Month (MM format) in the range of 1-12
     dateTime.tm_year =  (_u32)2023;        // Year (YYYY format)
     dateTime.tm_hour =  (_u32)16;          // Hours in the range of 0-23
     dateTime.tm_min =   (_u32)18;          // Minutes in the range of 0-59
     dateTime.tm_sec =   (_u32)00;          // Seconds in the range of  0-59
     sl_DeviceSet(SL_DEVICE_GENERAL,
               SL_DEVICE_GENERAL_DATE_TIME,
               sizeof(SlDateTime_t),
               (_u8 *)(&dateTime));

     sleep(3);

    _i16 configLen = sizeof(SlDateTime_t);
    _i8 configOpt = SL_DEVICE_GENERAL_DATE_TIME;
    sl_DeviceGet(SL_DEVICE_GENERAL,(_u8 *)&configOpt, (_u16 *)&configLen,(_u8 *)(&dateTime));
    printf("Day %d,Mon %d,Year %d,Hour %d,Min %d,Sec %d\n",dateTime.tm_day,dateTime.tm_mon,dateTime.tm_year,
            dateTime.tm_hour,dateTime.tm_min,dateTime.tm_sec);


    switch (start_retval) {
        case ROLE_STA:
            printf(">>ROLE_STA\n");
            GPIO_write(11, 1);   // Green
            break;

        case ROLE_RESERVED:
            printf(">>ROLE_RESERVED\n");
            GPIO_write(11, 1);   // Green
            break;

        case ROLE_AP:
            printf(">>ROLE_AP\n");
            GPIO_write(11, 1);   // Green
            break;

        case ROLE_P2P:
            printf(">>ROLE_P2P\n");
            GPIO_write(11, 1);   // Green
            break;

        case ROLE_TAG:
            printf(">>ROLE_TAG\n");
            GPIO_write(11, 1);   // Green
            break;

        default:
            printf(">>ERROR\n");
            GPIO_write(9, 1);   // Red
            break;
    }
    printf("start_retval: %d\n", start_retval);


    stop_retval = sl_Stop(SL_STOP_TIMEOUT);
    printf("stop_retval:  %d\n", stop_retval);

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
            // Need MCU device reset
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
