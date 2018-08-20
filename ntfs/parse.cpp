#include "parse.h"

parse::parse(int argc, char*argv[]){
    check_argc(argc,argv);
}

void parse::check_argc(int argc, char* argv[]){
    if(argc!=2){
        cout << " < Usage > : < File path > " << endl;
        exit(0);
    }
    this->file_path=argv[1];
    file_open_check();
}

void parse::file_open_check(){
    this->fp = fopen(this->file_path,"rb");
    if(!this->fp){
        perror("File Open Error !!\n");
        exit(0);
    }
    cout << " >> File Open Successed !!" << endl;
}

void parse::file_reading(){
    struct ntfs_struct ntfs_h;
    fread(&ntfs_h, 1, sizeof(ntfs_struct),this->fp);
    int jump_offset = static_cast<int>(ntfs_h.start_of_mft * ntfs_h.sector_per_cluster * ntfs_h.byte_per_sector);
    //cout << "jump_offset = " << jump_offset << endl; //temp
    mft_entry_parsing(jump_offset);
}

void parse::mft_entry_parsing(int jump_offset){
    fseek(this->fp, jump_offset, SEEK_SET);
    fread(&this->temp_cluster, 1, sizeof(this->temp_cluster),this->fp);
    this->temp=this->temp_cluster;
    struct mft_entry_header *mft_entry_h=(struct mft_entry_header*)this->temp;
    this->temp+=mft_entry_h->offset_to_first_attribute;
    struct common_attribute *common_attribute=(struct common_attribute*)(this->temp);
    uint8_t *runlist_offset, *runlist_length;   //use data attribute;

    while(common_attribute->attribute_type_id!=0){
            switch (common_attribute->attribute_type_id){
                case STANDARD_INFORMATION:
                {
                    struct standard_info_attribute *standard = (struct standard_info_attribute*)this->temp;
                    this->temp+=common_attribute->length_of_attribute;
                    common_attribute=(struct common_attribute*)this->temp;
                    cout << " [+]Standard Info Attribute Parsing Complete !!" << endl;
                }
                break;
                case ATTRIBUTE_LIST:
                break;
                case FILE_NAME :
                {
                    struct filename_attirbute *filename = (struct filename_attirbute*)this->temp;
                    this->temp+=common_attribute->length_of_attribute;
                    common_attribute=(struct common_attribute*)this->temp;
                    cout << " [+]File Name Attribute Parsing Complete !!" << endl;
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
                            this->jump_box+=jump_runlist; //
                            uint8_t cal_temp{0};   //use data attribute;
                            struct non_resident_header *n_resi_h=(struct non_resident_header*)this->temp;
                            this->temp-=sizeof(struct common_attribute); //Runlist offset 기준이 Data attribute 처음 기준이니까
                            this->temp+=n_resi_h->offset_to_the_runlist+this->jump_box;//지나온 runlist는 건너뜀
                            //jump_box를 쓰지않으면 this->temp가 재귀함수호출로 인하여 제대로 jump_runlist만큼 jump할 수 없음
                            
                            this->show_data(this->temp,30); //temp

                            memcpy(&cal_temp, this->temp,1);
                            this->offset = cal_temp >> 4;
                            this->length = cal_temp-((cal_temp >> 4) << 4);

                            runlist_offset = new uint8_t[this->offset];
                            runlist_length = new uint8_t[this->length];

                            memcpy(runlist_length, this->temp+1, static_cast<size_t>(this->length)); //runlist 바이트를 건너뛰고니까 +1 을 해줌
                            memcpy(runlist_offset, this->temp+1+this->length, static_cast<size_t>(this->offset)); //runlist 바이트를 건너뛰고니까 +1 을 해줌
                            memcpy(&this->last_runlist, this->temp+1+this->length+this->offset,1); //last bytes
                            this->jump_runlist = 1 + this->offset + this->length;


//                            show_data(runlist_length, this->length);//temp
//                            show_data(runlist_offset,this->offset); //temp
//                            printf("last runlist = %x\n",this->last_runlist);//temp
//                            printf("jump runlist = %d\n",this->jump_runlist);//temp



                            if(this->last_runlist==0x00){
                                delete [] runlist_length;
                                delete [] runlist_offset;
                                //cout << " [+]Data Attribute Parsing Complete !!" << endl;
                                cout << " >> [#]Parsing Finish !!" << endl;
                                exit(0);
                            }

                            int jump_offset = 0;                //runlist
                            jump_offset=runlist_offset[0];
                            for(int i=0; i < this->offset-1; i++){
                                jump_offset+=runlist_offset[i+1] << 8*(i+1);  //runlist offset 계산함 배열로 나누어 져있기 때문에 쉬프트연산을 이용하여 맞춰줌
                            }
//                            printf("Runlist  = 0x%x\n",jump_offset); //temp
                            cout << " [!]Jump to runlist offset !! " << endl;
                            mft_entry_parsing(jump_offset*4096); //끝나지 않았을 경우 재귀하여 다시시작
                        }
                        break;
                        default:
                            cout << " [-]Error to get resident flag !!" << endl;
                        break;
                    }
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
    for(int i=0; i<length; i++){
        if(i%16==0)
            cout << endl;
        printf("%02x ", data[i]);
    }
    cout << endl;
}
