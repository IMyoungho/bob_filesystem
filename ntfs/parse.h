#ifndef PARSE_H
#define PARSE_H
#include <iostream>
#include <arpa/inet.h>
#include <string.h>
#include "ntfs_header.h"

using namespace std;

class parse{
private:
    char * file_path;
    FILE *fp;
    uint8_t temp_cluster[4096];
    uint8_t *temp;
public:
    parse(int argc, char *argv[]);
    void check_argc(int argc, char *argv[]);
    void file_open_check();
    void file_reading();
    void mft_entry_parsing(int jump_offset);

    void show_data(uint8_t *data, int length);
};

#endif // PARSE_H
