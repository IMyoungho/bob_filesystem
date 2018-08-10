#include <iostream>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "ntfs_header.h"

using namespace std;

int main(int argc, char *argv[])
{
    //NTFS (UTF-16)
    if(argc!=2){
        perror("syntax error\n <usage> : <file path> \n");
        exit(1);
    }
    char *file_name=argv[1];
    FILE *fp=fopen(file_name,"r");
    if(!fp){
        perror("File open error~!\n");
        return -1;
    }

    struct ntfs_struct ns;
    fread(&ns,1,sizeof(ns),fp);
    cout << "file data copy finish" << endl;
    struct mft_entry_header meh;
    fseek(fp,0x2000,SEEK_SET);
    fread(&meh,1,sizeof(meh),fp);
    fseek(fp,0x2000+meh.offset_to_first_attribute,SEEK_SET);

    //go to attribute
    struct common_attribute common;
    fread(&common,1,sizeof(common),fp);

    //go to standard attribute
    struct standard_info_attribute standard;
    fread(&standard,1,sizeof(standard),fp);
    fseek(fp,common.length_of_attribute-(sizeof(standard)+sizeof(common)),SEEK_CUR);
    fread(&common,1,sizeof(common),fp);

    //go to filename attribute
    struct filename_attirbute filename;
    fread(&file_name,1,sizeof(filename),fp); //not read..;;temp

    fseek(fp,common.length_of_attribute-(sizeof(filename)+sizeof(common)),SEEK_CUR);
    fread(&common,1,sizeof(common),fp);
    cout << "test" << endl;
    fclose(fp);
    return 0;
}
