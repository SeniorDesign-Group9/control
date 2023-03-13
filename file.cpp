// file.cpp

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/net/wifi/fs.h>
#include "ti_drivers_config.h"
#include "file.hh"

// Constructor
FileManager::FileManager(std::string fn) {
    file_name = fn;
}

// Deconstructor
FileManager::~FileManager() {}

// Class functions
std::string FileManager::getFileName(void) {
    return file_name;
}

int32_t FileManager::writeToFile(std::string line) {
    char            dev_file_name[BUFFER_LEN] = {0};
    int         retval = 0;
    long            dev_file_handle = -1;
    uint32_t        master_token = 0;
    unsigned long   max_size = 63 * 1024; // 62.5K is max file size
    unsigned long   offset = 0;
    unsigned char   input_buffer[BUFFER_LEN] = {0};
    int i = 0;

    // Input line is too long
    if (line.length() >= BUFFER_LEN) {
        return -1;
    }



    // Copy C string version of file_name to our device file name
    strcpy(dev_file_name, file_name.c_str());

    // Copy C string version of line to our input buffer
    std::memcpy(input_buffer, reinterpret_cast<const unsigned char*>(line.c_str()), BUFFER_LEN);

    // Create a file and write data
    dev_file_handle = sl_FsOpen((unsigned char *)dev_file_name, SL_FS_CREATE | SL_FS_OVERWRITE | SL_FS_CREATE_FAILSAFE | SL_FS_CREATE_MAX_SIZE(max_size), 0);

    printf("dev_file_handle:   %ld\n", dev_file_handle);

    //      (unsigned char *)dev_file_name, SL_FS_CREATE|SL_FS_OVERWRITE|SL_FS_CREATE_PUBLIC_WRITE|SL_FS_CREATE_PUBLIC_READ|SL_FS_CREATE_NOSIGNATURE|SL_FS_CREATE_MAX_SIZE(max_size), (unsigned long *)&master_token);


    offset = 0;
    // Preferred in secure file that the offset and the length will be aligned to 16 bytes
    retval = sl_FsWrite(dev_file_handle, offset, input_buffer, static_cast<uint32_t>(BUFFER_LEN));
    printf("retval sl_FsWrite: %d\n", retval);
    retval = sl_FsClose(dev_file_handle, NULL, 0, 0);
    printf("retval sl_FsClose: %d\n", retval);
    if (retval < 0) {
        // Abort
        retval = sl_FsClose(dev_file_handle,0,(unsigned char *)'A',1);
        printf("retval Abort:      %d\n", retval);
    }


    // Reset input buffer
    for (i = 0; i < BUFFER_LEN; i++) {
        input_buffer[i] = 0;
    }

    // Open the same file for read, using the Token we got from the creation procedure above   (unsigned long *)&master_token
    dev_file_handle =  sl_FsOpen((unsigned char *)dev_file_name, SL_FS_READ, 0);
    printf("dev_file_handle:   %ld\n", dev_file_handle);


    offset = 0;
    retval = sl_FsRead( dev_file_handle, offset, (unsigned char * )input_buffer, line.length());
    printf("retval sl_FsRead:  %d\n", retval);
    retval = sl_FsClose(dev_file_handle, 0, 0, 0);
    printf("retval sl_FsClose: %d\n", retval);

    // Print input buffer
    printf("Output:\n");

    i = 0;
    while (input_buffer[i] != '\0' || i < BUFFER_LEN) {
        std::cout << input_buffer[i];
        i++;
    }
    std::cout << std::endl;

    return retval;
}

void FileManager::readFileToConsole(void) {

}
