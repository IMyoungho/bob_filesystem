#ifndef PARSE_H
#define PARSE_H
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <arpa/inet.h>

using namespace std;

#define BOOT_CODE 446
#define SECTOR_SIZE 512
#define GPT_PARTITION_MAX_NUM 128

#pragma pack(push,1)
struct partion {
    uint8_t start;
    uint8_t chs_addr[3];
    uint8_t partion_type;
    uint8_t chs_addr2[3];
    uint32_t LBA_addr_start;
    uint32_t partion_size;
};
struct mbr{
    uint8_t mbr_data[BOOT_CODE];
    struct partion p1[4];
    uint16_t sig;
};
struct gpt{
    uint8_t partition_type_guid[16];
    uint8_t unique_partition_guid[16];
    uint64_t first_LBA;
    uint64_t last_LBA;
    uint64_t attribute;
    uint8_t partition_name[72];
};
#pragma pack(pop)
class parse {
private:
    char *mbr_file_path;
    char *gpt_file_path;
public:
    parse(int argc, char *argv[]);
    void check_argc(int argc, char *argv[]);
    void file_open();
    void print_mbr_locate_size(mbr mb, int i);
    void print_gpt_locate_size(gpt gp);
};
#endif // PARSE_H
