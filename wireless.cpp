// wireless.cpp

// Includes
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <pthread.h>
#include <unistd.h>

#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/net/wifi/device.h>
#include <ti/drivers/net/wifi/netapp.h>
#include <ti/drivers/net/wifi/simplelink.h>
#include <ti/drivers/net/wifi/wlan.h>

#include "ti_drivers_config.h"
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
// Start and provision NWP
int Wireless::start(void) {
    int32_t retval = 0;
    uint8_t pConfigOpt = 0;         // FIXME debug
    uint16_t pConfigLen = 0;        // FIXME debug
    SlDeviceVersion_t ver = {0};    // FIXME debug


    // Set NWP to state defined in sysconfig
    retval = sl_WifiConfig();
    if (retval < 0) {
        printf("sl_WifiConfig error%d\n", retval);
        return retval;
    }

    // Start NWP
    retval = sl_Start(0, 0, 0);
    if (retval < 0) {
        printf("sl_Start error%d\n", retval);
        return retval;
    }

    // FIXME debug get chip ver
    pConfigLen = sizeof(ver);
    pConfigOpt = SL_DEVICE_GENERAL_VERSION;
    sl_DeviceGet(SL_DEVICE_GENERAL, &pConfigOpt, &pConfigLen, (uint8_t *)(&ver));
    printf("CHIP 0x%x\nMAC 31.%d.%d.%d.%d\nPHY %d.%d.%d.%d\nNWP %d.%d.%d.%d\nROM %d\nHOST %d.%d.%d.%d\n",
            ver.ChipId,
            ver.FwVersion[0],ver.FwVersion[1],
            ver.FwVersion[2],ver.FwVersion[3],
            ver.PhyVersion[0],ver.PhyVersion[1],
            ver.PhyVersion[2],ver.PhyVersion[3],
            ver.NwpVersion[0],ver.NwpVersion[1],ver.NwpVersion[2],ver.NwpVersion[3],
            ver.RomVersion,
            SL_MAJOR_VERSION_NUM,SL_MINOR_VERSION_NUM,SL_VERSION_NUM,SL_SUB_VERSION_NUM);


    // Set device name
    retval = sl_NetAppSet(SL_NETAPP_DEVICE_ID, SL_NETAPP_DEVICE_URN, strlen(device_name), (uint8_t *)device_name);
    if (retval < 0) {
        printf("set SL_NETAPP_DEVICE_URN error %d\n", retval);
        return retval;
    }

    // Set domain name
    retval = sl_NetAppSet(SL_NETAPP_DEVICE_ID, SL_NETAPP_DEVICE_DOMAIN, strlen(domain_name), (uint8_t *)domain_name);
    if (retval < 0) {
        printf("set SL_NETAPP_DEVICE_ID error%d\n", retval);
        return retval;
    }

    // Set SC provisioning
    retval = sl_WlanProvisioning((uint8_t)SL_WLAN_PROVISIONING_CMD_START_MODE_APSC,
                                 (uint8_t)ROLE_STA,
                                 (uint16_t)PROVISIONING_INACTIVITY_TIMEOUT,
                                 NULL,
                                 (uint32_t)NULL);
    if(retval < 0) {
        printf("set SL_WLAN_PROVISIONING_CMD_START_MODE_APSC_EXTERNAL_CONFIGURATION error %d\n", retval);
        return retval;
    }

    // FIXME debug: stop provisioning
    //sl_WlanProvisioning(SL_WLAN_PROVISIONING_CMD_STOP,0xFF,0,NULL, 0x0);


    return 0;
}

// Stop NWP
int Wireless::stop(void) {
    int16_t stop_retval = 0;

    stop_retval = sl_Stop(SL_STOP_TIMEOUT);

    return stop_retval;
}

int Wireless::haltProvisioning(void) {
    uint32_t statusWlan;
    uint16_t pConfigLen;
    uint8_t pConfigOpt;
    pConfigOpt = SL_DEVICE_EVENT_CLASS_WLAN;
    pConfigLen = sizeof(uint32_t);
    sl_WlanProvisioning(SL_WLAN_PROVISIONING_CMD_STOP, 0xFF, 0, NULL, 0x0);

    while(SL_WLAN_EVENT_PROVISIONING_STATUS & statusWlan) {
        sl_DeviceGet(SL_DEVICE_STATUS,&pConfigOpt,&pConfigLen,(_u8 *)(&statusWlan));
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
