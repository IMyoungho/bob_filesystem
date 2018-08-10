#include "parse.h"

parse::parse(int argc, char *argv[]){
    check_argc(argc,argv);
}

void parse::check_argc(int argc, char *argv[]){
    if(argc!=3){
        cout << "<Usage> : <MBR FILE PATH> <GPT FILE PATH>" << endl;
        exit(1);
    }
    this->mbr_file_path = argv[1];
    this->gpt_file_path = argv[2];
}
void parse::print_mbr_locate_size(struct mbr mb, int i){
    printf(">> Start locate = %d\n", mb.p1[i].LBA_addr_start);
    printf(">Partition Size = %d\n", mb.p1[i].partion_size*SECTOR_SIZE/1024/1024);
};
void parse::print_gpt_locate_size(struct gpt gp){
        cout << "start = " << dec << gp.first_LBA;
    cout << endl;
        cout  << "last  = " << dec << gp.last_LBA;
    cout << endl;
        cout << "size  = " << dec << (gp.last_LBA-gp.first_LBA+1)*512/1024/1024 << endl;
}
void parse::file_open(){
    FILE *fp;
    cout << "< MBR INFORMATION >\n" << endl;
    //-------------------------MBR---------------------------
    fp=fopen(this->mbr_file_path, "rb");
    struct mbr mb;
    fread(&mb, 1, sizeof(mb), fp);
    int i=0;
    while(fp){
        if(mb.p1[i].partion_type==0x07){
            print_mbr_locate_size(mb,i);
        }
//        else if(mb.p1[i].partion_type==0x05){
//            print_mbr_locate_size(mb,i);
//            cout << " < Extra partion is exist > " << endl;
//            fseek(fp,(mb.p1[i].LBA_addr_start*SECTOR_SIZE),SEEK_SET);
//            fread(&mb,1,sizeof(mb),fp);
//            i=0;
//            while(mb.p1[i].partion_type!=0x00){
//                    print_mbr_locate_size(mb,i);
//                    i++;
//            }
//        }
        else
            break;
        i++;
    }
    cout << "\n< GPT INFORMATION >" << endl;
    //-------------------------GPT---------------------------
    fp=fopen(this->gpt_file_path,"rb");
    struct gpt gp;
    fseek(fp,SECTOR_SIZE*2,SEEK_SET);
    fread(&gp, 1, sizeof(gp), fp);
    for(int i=0; i<GPT_PARTITION_MAX_NUM; i++){
        if(fp&&gp.first_LBA!=0 && gp.last_LBA!=0){
            print_gpt_locate_size(gp);
            fread(&gp, 1, sizeof(gp), fp);
        }
    }
}

