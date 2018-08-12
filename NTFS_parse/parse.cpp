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

    struct ntfs_struct ntfs_h;
    fread(&ntfs_h,1,sizeof(ntfs_struct),this->fp);
    cout << "  >> NTFS data copy finish!!" << endl;
    int jump_offset=static_cast<int>(ntfs_h.byte_per_sector * ntfs_h.sector_per_cluster * ntfs_h.start_of_mft);
    mft_entry_parsing(jump_offset);

}

void parse::mft_entry_parsing(int jump_offset){
    struct mft_entry_header entry_h;
    fseek(this->fp,jump_offset,SEEK_SET); // 512 * 8 * 0x1aaa -> mtf real postion
    fread(&entry_h,1, sizeof(mft_entry_header),this->fp);
    fseek(this->fp, entry_h.offset_to_first_attribute-sizeof(mft_entry_header),SEEK_CUR);
    struct common_attribute common;
    attribute_parsing(common);
}
void parse::attribute_parsing(struct common_attribute common){

    uint8_t *off_data;
    uint8_t *len_data;
    int offset;
    int length;
    fread(&common,1,sizeof(struct common_attribute),this->fp);
    while(common.attribute_type_id!=0){
        switch (common.attribute_type_id){
            case STANDARD_INFORMATION: //go function
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
                            uint8_t temp{0};
                            struct non_resident_header non_resident;
                            fread(&non_resident,1,sizeof(non_resident_header),this->fp);
                            while(true){
                                //다시돌아왔을때 읽었던 Runlist는 건너뛰어야댐!!
                                fseek(this->fp,this->go_runlist,SEEK_CUR);
                                fread(&temp,1,1,this->fp); //runlist info
                                offset = temp >> 4;        //divide runlist
                                length = temp-((temp >> 4) << 4);
                                len_data = new uint8_t[length];
                                off_data = new uint8_t[offset];

                                this->go_runlist=offset+length+1; // 1 = runlist data
                                fread(len_data,1,static_cast<size_t>(length),this->fp);
                                fread(off_data,1,static_cast<size_t>(offset),this->fp);
                                uint8_t last_check=0;
                                fread(&last_check,1,1,this->fp);

//                                cout << "len data = ";
//                                for(int i=0; i<length; i++)
//                                {
//                                    printf(" 0x%02x ", len_data[i]);
//                                }
//                                cout << "\noffset data = ";
//                                for(int i=0; i<offset; i++)
//                                {
//                                    printf(" 0x%02x ", off_data[i]);
//                                }
//                                cout << endl;

                                fseek(this->fp,-1,SEEK_CUR); //마지막 부분을 읽었으니 다시 한칸 뒤로가야 정상적으로 읽을 수 있음
                                int jump_offset = 0;
                                jump_offset=off_data[0];
                                for(int i=0; i<offset-1; i++){
                                    jump_offset+=off_data[i+1] << 8*(i+1);
                                }
                                cout << "Runlist = "<< hex << jump_offset * 4096 << endl;
                                if(last_check==0x00){ // last data check!!
                                    cout << "   >> End of Cluster Bye Bye ~ !!" << endl;
                                    delete [] off_data;
                                    delete [] len_data;
                                    exit(0);
                                }
                                mft_entry_parsing(jump_offset*4096);//다시 MFT Entry로.. 재귀함수
                            }
                        }
                        //break; //because no excute
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
