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

void parse::file_reading(){
    struct ntfs_struct ntfs_h;
    fread(&ntfs_h, 1, sizeof(ntfs_struct),this->fp);
    int jump_offset = static_cast<int>(ntfs_h.start_of_mft * ntfs_h.sector_per_cluster * ntfs_h.byte_per_sector);
    mft_entry_parsing(jump_offset);
    cout << "jump_offset = " << jump_offset << endl;
}

void parse::mft_entry_parsing(int jump_offset){
    fseek(this->fp, jump_offset, SEEK_SET);
    fread(&this->temp_cluster, 1, sizeof(this->temp_cluster),this->fp);
    this->temp=this->temp_cluster;
    struct mft_entry_header *mft_entry_h=(struct mft_entry_header*)this->temp;
    this->temp+=mft_entry_h->offset_to_first_attribute;
    struct common_attribute *common_attribute=(struct common_attribute*)(this->temp);

    uint8_t *runlist_offset, *runlist_length;   //use data attribute;
    int offset{0}, length{0};                   //use data attribute;

    while(common_attribute->attribute_type_id!=0){
            switch (common_attribute->attribute_type_id){
                case STANDARD_INFORMATION:
                {
                    struct standard_info_attribute *standard = (struct standard_info_attribute*)this->temp;
                    this->temp+=common_attribute->length_of_attribute;
                    common_attribute=(struct common_attribute*)this->temp;
                    cout << " [+]Standard Info Attribute Parsing Complete!!" << endl;
                }
                break;
                case ATTRIBUTE_LIST:
                break;
                case FILE_NAME :
                {
                    struct filename_attirbute *filename = (struct filename_attirbute*)this->temp;
                    this->temp+=common_attribute->length_of_attribute;
                    common_attribute=(struct common_attribute*)this->temp;
                    cout << " [+]File Name Attribute Parsing Complete!!" << endl;
                }
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
                {
                    this->temp+=sizeof(struct common_attribute);
                    switch (common_attribute->non_resident_flag) {
                        case RESIDENT:
                        {
                            struct resident_header *resi_h=(struct resident_header*)this->temp;
                        }
                        break;
                        case NON_RESIDENT:
                        {
                            uint8_t cal_temp{0};   //use data attribute;
                            struct non_resident_header *n_resi_h=(struct non_resident_header*)this->temp;
                            this->temp-=sizeof(struct common_attribute); //Runlist offset 기준이 Data attribute 처음 기준이니까
                            this->temp+=n_resi_h->offset_to_the_runlist;
                            this->show_data(this->temp,30);
                            memcpy(&cal_temp, this->temp,1);
                            printf("cal_temp = %d", cal_temp);

                        }
                        break;
                        default:
                            cout << " [-]Error to get resident flag!!" << endl;
                        break;
                    }
                    cout << " [+]Data Attribute Parsing Complete!!" << endl;
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

void parse::show_data(uint8_t *data, int length){
    for(int i=0; i<length; i++)
    {
        if(i%16==0)
            cout << endl;
        printf("%02x ", data[i]);
    }
    cout << endl;
}
