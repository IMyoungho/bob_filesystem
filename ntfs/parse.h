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
    uint8_t temp_cluster[4096];              //파일을 클러스터단위로 읽어들이기위한 버퍼
    uint8_t *temp;                           //클러스터배열의 위치 포인터
    int offset{0}, length{0};                //use data attribute;
    int jump_runlist{0};                     //use data attribute; 다시 runlist 계산시 건너뛰어야하는 길이
    uint8_t last_runlist{0};                 //use data attribute; runlist 마지막 바이트
    int jump_box;                            //jump runlist를 차곡차곡 모아놓음
public:
    parse(int argc, char *argv[]);
    void check_argc(int argc, char *argv[]);
    void file_open_check();
    void file_reading();
    void mft_entry_parsing(int jump_offset);
    void show_data(uint8_t *data, int length);
};

#endif // PARSE_H
