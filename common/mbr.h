#ifndef _MBR_H_
#define _MBR_H_

#include <stdint.h>

typedef struct __attribute__((packed)) _partition {
    uint8_t attributes;
    uint8_t start_chs[3];
    uint8_t type;
    uint8_t end_chs[3];
    uint32_t lba_start;
    uint32_t num_sectors;
} partition_t;

typedef struct __attribute__((packed)) _mbr {
    uint8_t bootstrap[0x1B8];
    uint8_t disk_id[4];
    uint8_t reserved[2];
    partition_t partitions[4];
    uint8_t signature[2];
} mbr_t;

#endif // _MBR_H_
