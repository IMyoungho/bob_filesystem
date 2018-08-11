#ifndef PARSE_H
#define PARSE_H
#include <iostream>
#include "ntfs_header.h"

using namespace std;

class parse{
private:
    char *file_path;
    FILE *fp;
public:
    parse(int argc, char *argv[]);
    void check_argc(int argc, char* argv[]);
    void file_open_check();
    void reading_parsing_file();
    void jump_and_get_common();

};

#endif // PARSE_H
