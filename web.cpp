// web.cpp

// Includes
#include <cstdio>
#include <cstdint>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <ti/net/http/httpserver.h>
#include <ti/net/http/urlhandler.h>

#include <ti/drivers/net/wifi/device.h>
#include <ti/drivers/net/wifi/fs.h>
#include <ti/drivers/net/wifi/netapp.h>
#include <ti/drivers/net/wifi/simplelink.h>
#include <ti/drivers/net/wifi/wlan.h>
#include "web.hh"

#include "ti_drivers_config.h"

// Singleton functions
// Empty constructor
WebServer::WebServer() {}

// Empty destructor
WebServer::~WebServer() {}

// Instance
WebServer& WebServer::instance() {
    static WebServer inst;
    return inst;
}

// Start HTTP server
int WebServer::start(void) {
    int16_t retval = 0;

    // SL_ERROR_NET_APP_HTTP_SERVER_ALREADY_STARTED is -6214
    //retval = sl_NetAppStart(SL_NETAPP_HTTP_SERVER_ID);

    if (retval < 0) {
        printf("sl_NetAppStart error %d\n", retval);
    }

    // Open file and read
    char*           DeviceFileName = "www/gardenirs.html";
    int32_t         DeviceFileHandle = -1;
    unsigned char   readBuff[100] = {0};
    uint32_t        MasterToken = 0;
    uint32_t        readSize = 0;
    int             i = 0;

    // SL_ERROR_FS_FILE_NOT_EXISTS is -10341
    DeviceFileHandle = sl_FsOpen((unsigned char *)DeviceFileName,
                                 SL_FS_READ,  (unsigned long *)(&MasterToken));
    printf("sl_FsOpen: %d\n", DeviceFileHandle);
    retval = sl_FsRead(DeviceFileHandle, 0, &readBuff[0], readSize);
    printf("sl_FsRead: %d\n", retval);

    for (i = 0; i < 100; i++) {
        printf("%c", readBuff[i]);
    }
    printf("> EOF\n");

    retval = sl_FsClose(DeviceFileHandle, NULL, NULL , 0);


    /*

    unsigned char   DeviceFileName[] = "MyFile.txt";
    uint32_t        MasterToken = 0, MaxSize = 63 * 1024; //62.5K is max file size
    int32_t         DeviceFileHandle = -1;

    _i32            RetVal;        //negative retval is an error
    unsigned long   Offset = 0;
    unsigned char   InputBuffer[100] = {0};

    // Create a file and write data. The file in this example is secured, without signature and with a fail safe commit
    //create a secure file if not exists and open it for write.
    DeviceFileHandle =  sl_FsOpen((unsigned char *)DeviceFileName,
                                  SL_FS_CREATE | SL_FS_OVERWRITE | SL_FS_CREATE_SECURE | SL_FS_CREATE_NOSIGNATURE | SL_FS_CREATE_MAX_SIZE( MaxSize ),
                                  (unsigned long *)(&MasterToken));
    printf("sl_FsOpen: %d\n", DeviceFileHandle);


    Offset = 0;
    //Preferred in secure file that the Offset and the length will be aligned to 16 bytes.
    RetVal = sl_FsWrite( DeviceFileHandle, Offset, (unsigned char *)"HelloWorld", strlen("HelloWorld"));
    printf("sl_FsWrite: %d\n", RetVal);

    RetVal = sl_FsClose(DeviceFileHandle, NULL, NULL , 0);
    printf("sl_FsClose: %d\n", RetVal);
    // open the same file for read, using the Token we got from the creation procedure above
    DeviceFileHandle =  sl_FsOpen((unsigned char *)DeviceFileName,
                                  SL_FS_READ,
                                  (unsigned long *)(&MasterToken));
    printf("sl_FsOpen: %d\n", DeviceFileHandle);
    Offset = 0;
    RetVal = sl_FsRead( DeviceFileHandle, Offset, (unsigned char *)InputBuffer, strlen("HelloWorld"));
    printf("sl_FsWrite: %d\n", RetVal);
    RetVal = sl_FsClose(DeviceFileHandle, NULL, NULL , 0);
    printf("sl_FsClose: %d\n", RetVal);
    printf("buffer: <%s>\n", (unsigned char *)InputBuffer);
    */

    return 0;
}

// Stop HTTP server
int WebServer::stop(void) {
    int16_t retval = 0;

    retval = sl_NetAppStop(SL_NETAPP_HTTP_SERVER_ID);

    if (retval < 0) {
        printf("sl_NetAppStop error\n");
    }

    return 0;
}
