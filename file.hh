// file.hh

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <cstdint>

#define BUFFER_LEN      100     // Input buffer length

// File system class
class FileManager {
    public:
        // Constructor/destructor
        FileManager(std::string fn);
        ~FileManager();

        // Class functions
        std::string getFileName(void);
        int32_t writeToFile(std::string line);
        int32_t readFileToConsole(void);

    
    private:
        // Class variables
        std::string file_name;
};

#endif // FILEMANAGER_H
