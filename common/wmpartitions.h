#ifndef _WMPARTITIONS_H_
#define _WMPARTITIONS_H_

#include <stdint.h>

typedef struct __attribute__((packed)) _wmstore_hdr {
    char magic[8];
    uint8_t guid[0x10];
    int16_t name[0x20]; // wchar_t, 0x40
    uint32_t unk1;
    uint32_t unk2;
    uint32_t unk3;
    uint64_t store_timestamp;
    uint32_t unk5;
    uint8_t padding[0x190];
} wmstore_hdr_t;

typedef struct __attribute__((packed)) _wmpart_hdr {
    char magic[8];
    int16_t name[0x20]; // wchar_t, 0x40
    uint32_t unk1;
    uint32_t offset_sector;
    uint32_t unk2;
    uint32_t size_sectors;
    uint32_t unk3;
    uint64_t part_timestamp; // matches wmstore.store_timestamp
    uint32_t partition_type;
    uint32_t unk5;
    uint32_t unk6;
    uint8_t padding[0x190];
} wmpart_hdr_t;

#endif // _WMPARTITIONS_H_
