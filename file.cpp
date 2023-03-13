// file.cpp

#include <cstdint>
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
    char            dev_file_name[BUFFER_LEN];
    int32_t         retval;
    long            dev_file_handle = -1;
    uint32_t        master_token = 0;
    unsigned long   max_size = 63 * 1024; // 62.5K is max file size
    unsigned long   offset = 0;
    unsigned char   input_buffer[BUFFER_LEN];

    // Input line is too long
    if (line.length() >= BUFFER_LEN) {
        return -1;
    }



    // Copy C string version of file_name to our device file name
    strcpy(dev_file_name, file_name.c_str());

    // Copy C string version of line to our input buffer
    std::memcpy(input_buffer, reinterpret_cast<const unsigned char*>(line.c_str()), BUFFER_LEN);

    // Create a file and write data
    dev_file_handle = sl_FsOpen((unsigned char *)dev_file_name,
                                SL_FS_CREATE|SL_FS_OVERWRITE|SL_FS_CREATE_SECURE|SL_FS_CREATE_NOSIGNATURE|SL_FS_CREATE_MAX_SIZE(max_size),
                                (unsigned long *)&master_token);

    offset = 0;
    // Preferred in secure file that the offset and the length will be aligned to 16 bytes
    retval = sl_FsWrite(dev_file_handle, offset, input_buffer, static_cast<uint32_t>(BUFFER_LEN));
    retval = sl_FsClose(dev_file_handle, NULL, NULL , 0);

    // Open the same file for read, using the Token we got from the creation procedure above
    dev_file_handle =  sl_FsOpen((unsigned char *)dev_file_name, SL_FS_READ, (unsigned long *)&master_token);

    offset = 0;
    retval = sl_FsRead( dev_file_handle, offset, (unsigned char * )input_buffer, line.length());
    retval = sl_FsClose(dev_file_handle, NULL, NULL , 0);

    return retval;
}

void FileManager::readFileToConsole(void) {

}
