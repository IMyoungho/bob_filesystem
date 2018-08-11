#ifndef NTFS_HEADER_H
#include <iostream>

#define NTFS_HEADER_H
#define STANDARD_INFORMATION 0x10
#define ATTRIBUTE_LIST 0x20
#define FILE_NAME 0x30
#define VOLUME_VERSION_OBJECT_ID 0x40
#define SECURITY_DISCRIPTOR 0x50
#define VOLUME_NAME 0x60
#define VOLUME_INFORMATION 0x70
#define DATA 0x80
#define INDEX_ROOT 0x90
#define INDEX_ALLOCATE 0xA0
#define BITMAP 0xB0
#define SYSTEM_LINK_REPARSE 0xC0
#define EA_INFORMATION 0xD0
#define EA 0xE0
#define LOGGED_UTILITY_STREAM 0x100

#define RESIDENT 0
#define NON_RESIDENT 1

using namespace std;

#pragma pack(push, 1)
struct ntfs_struct{
    uint8_t jump_boot_code[3];
    uint64_t OEM_name;
    uint16_t byte_per_sector;
    uint8_t sector_per_cluster;
    uint16_t reserved_sector_count;
    uint8_t reserved1[5];
    uint8_t media;
    uint8_t reserved2[18];
    uint64_t total_sectors;
    uint64_t start_of_mft;
    uint64_t start_of_mftmirr;
    uint8_t mft_entry_size;
    uint8_t reserved3[3];
    uint8_t index_record_size;
    uint8_t no_data[3];
    uint64_t serial_number;
    uint8_t reserved4[430];
    uint16_t signature;
};
struct mft_entry_header{ //Master File Table
    uint32_t signature;
    uint16_t offset_fixup_array;
    uint16_t count_of_fixup_value;
    uint64_t log_file_sequence_number;
    uint16_t sequence_value;
    uint16_t hard_link_count;
    uint16_t offset_to_first_attribute;
    uint16_t flags;
    uint32_t used_size_of_mft_entry;
    uint32_t allocated_size_of_mft_entry;
    uint64_t file_reference_to_base_mft_entry;
    uint16_t next_attribute_id;
    //uint16_t align_boundary;
    //uint64_t mft_record_number;
    //uint16_t  ?
    //uint32_t  ?
    //uint16_t  ?
};
struct common_attribute{  //attribute
    uint32_t attribute_type_id;
    uint32_t length_of_attribute;
    uint8_t non_resident_flag;
    uint8_t length_of_name;
    uint16_t offset_to_name;
    uint16_t flags;
    uint16_t attribute_identifier;
};
struct standard_info_attribute{
    uint64_t creation_time;
    uint64_t modified_time;
    uint64_t mft_modifed_time;
    uint64_t accessed_time;
    uint32_t flags;//dos permission?
    uint32_t maximum_number_of_ver;
    uint32_t ver_number;
    uint32_t class_id;
    uint32_t owner_id;
    uint32_t security_id;
    uint64_t quota_charge;
    uint64_t usn;
};
struct filename_attirbute{
    uint64_t parent_file_reference_address;
    uint64_t create_time;
    uint64_t modify_time;
    uint64_t mft_modified_time;
    uint64_t accessed_time;
    uint64_t allocated_size_of_file;
    uint64_t real_size_of_file;
    uint32_t flags;
    uint32_t reparse_value;
    uint8_t length_of_name;
    uint8_t name_space;
    //uint8_t *name;
};
struct resident_header{ //data attribute
    uint32_t size_of_content;
    uint16_t offset_to_content;
    uint8_t indexed_flag;
    uint8_t padding;
};
struct non_resident_header{ //data attribute
    uint64_t starting_vcn_of_the_runlist;
    uint64_t ending_vcn_of_the_runlist;
    uint16_t offset_to_the_runlist;
    uint16_t compression_unit_size;
    uint32_t padding;
    uint64_t allocated_size_of_attribute_content;
    uint64_t real_size_of_attribute_content;
    uint64_t initialized_size_of_attribute_content;
};
#pragma pack(pop)
#endif // NTFS_HEADER_H

