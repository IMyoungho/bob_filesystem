#ifndef PARSE_H
#define PARSE_H
#include <iostream>
#include "ntfs_header.h"

using namespace std;

class parse{
private:
    char *file_path;
    FILE *fp;
    int go_runlist=0;
public:
    parse(int argc, char *argv[]);
    void check_argc(int argc, char* argv[]);
    void file_open_check();
    void reading_parsing_file();
    void mft_entry_parsing(int jump_offset);
    void attribute_parsing(common_attribute common);
};

#endif // PARSE_H
