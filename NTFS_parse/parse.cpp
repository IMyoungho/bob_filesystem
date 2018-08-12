#include "parse.h"

parse::parse(int argc, char*argv[]){
    check_argc(argc,argv);
}


void parse::check_argc(int argc, char* argv[]){
    if(argc!=2){
        cout << " <usage> : <file path> " <<endl;
        exit(0);
    }
    this->file_path=argv[1];
    file_open_check();
}


void parse::file_open_check(){
    this->fp = fopen(this->file_path,"rb");
    if(!this->fp){
        perror("File Open Error!!\n");
        exit(0);
    }
    cout << " >> File Open Successed !!" << endl;
}


void parse::reading_parsing_file(){
    uint8_t temp;
    struct ntfs_struct ntfs_h;
    fread(&ntfs_h,1,sizeof(ntfs_struct),this->fp);
    cout << "ntfs data copy finish!!" << endl;

    struct mft_entry_header entry_h;
    fseek(this->fp,static_cast<int>(ntfs_h.byte_per_sector * ntfs_h.sector_per_cluster * ntfs_h.start_of_mft),SEEK_SET); // 512 * 8 * 0x1aaa -> mtf real postion
    fread(&entry_h,1, sizeof(mft_entry_header),this->fp);
    fseek(this->fp, entry_h.offset_to_first_attribute-sizeof(mft_entry_header),SEEK_CUR);

    struct common_attribute common;
    fread(&common,1,sizeof(struct common_attribute),this->fp);
    while(common.attribute_type_id!=0){
        switch (common.attribute_type_id){
            case STANDARD_INFORMATION:
                struct standard_info_attribute standard;
                fread(&standard,1,sizeof(standard_info_attribute),this->fp);
                fseek(this->fp,common.length_of_attribute-(sizeof(standard_info_attribute)+sizeof(common_attribute)),SEEK_CUR);
                fread(&common,1,sizeof(common_attribute),this->fp);
                break;
            case ATTRIBUTE_LIST:

                break;
            case FILE_NAME :
                struct filename_attirbute filename;
                fread(&filename,1,sizeof(filename_attirbute),this->fp);
                fseek(this->fp,common.length_of_attribute-(sizeof(filename_attirbute)+sizeof(common_attribute)),SEEK_CUR);
                fread(&common,1,sizeof(common_attribute),this->fp);
                break;
            case VOLUME_VERSION_OBJECT_ID:

                break;
            case SECURITY_DISCRIPTOR:

                break;
            case VOLUME_NAME:

                break;
            case VOLUME_INFORMATION:

                break;
            case DATA:
                switch (common.non_resident_flag){
                    case RESIDENT:{
                        struct resident_header resident;
                        fread(&resident,1,sizeof(resident_header),this->fp);
                        }
                        break;
                    case NON_RESIDENT:{
                        struct non_resident_header non_resident;
                        fread(&non_resident,1,sizeof(non_resident_header),this->fp);
                        fread(&temp,1,1,this->fp);
                        int offset = temp >> 4;
                        int length = temp-((temp >> 4) << 4);
                        cout<< offset << " " << length <<endl;

                        uint8_t *off = new uint8_t[offset];
                        uint8_t *len = new uint8_t[length];
                        fread(len,1,static_cast<size_t>(length),this->fp);
                        fread(off,1,static_cast<size_t>(offset),this->fp);

                        delete [] off;
                        delete [] len;
                        }
                        break;
                    default:
                        break;
                }
                break;
            case INDEX_ROOT:

                break;
            case INDEX_ALLOCATE:

                break;
            case BITMAP:

                break;
            case SYSTEM_LINK_REPARSE:

                break;
            case EA_INFORMATION:

                break;
            case EA:

                break;
            case LOGGED_UTILITY_STREAM:

                break;

            default:
                break;
        }
    }
}
