#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

#include "../common/guid.h"
#include "../common/mbr.h"
#include "../common/wmpartitions.h"

#define SECTOR_SIZE 0x200

void print_guid(uint8_t *guid) {
    guid_t *id = (guid_t *)guid;
    printf("%08x-%02x-%02x-%02x-%02x%02x%02x%02x%02x%02x\n",
        id->p1, id->p2, id->p3, id->p4,
        id->p5[0], id->p5[1], id->p5[2], id->p5[3], id->p5[4], id->p5[5]);
}

void hexdump(uint8_t *data, int len) {
    for (int i = 0; i < len; i++)
        printf("%02x", data[i]);
    printf("\n");
}

// i didn't want to do it like this, but wchar_t wasn't playing ball
// and it's very unlikely a partition name would be incorrect when run through this
// so let's just try it and see
char *cheap_wchar_to_ascii(int16_t *chars) {
    int i = 0;
    static char asciibuf[0x40];
    memset(asciibuf, 0, sizeof(asciibuf));
    while (*chars != 0 && i < sizeof(asciibuf)) {
        asciibuf[i] = (char)*chars;
        chars++;
        i++;
    }
    return asciibuf;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("usage: %s path/to/image\n", argv[0]);
        return -1;
    }

    // try to open the file
    FILE *fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        printf("failed to open file\n");
        return -1;
    }

    // read the mbr-like header
    // TODO(Emma): it's not quite MBR so we should probably figure out what this actually is
    mbr_t part_hdr;
    if (fread(&part_hdr, sizeof(part_hdr), 1, fp) != 1) {
        printf("failed to read header\n");
        fclose(fp);
        return -1;
    }

    // verify a few values
    if (part_hdr.partitions[0].type != 0xEF) {
        printf("partition header has type 0x%02X instead of 0xEF\n", part_hdr.partitions[0].type);
        fclose(fp);
        return -1;
    }
    if (part_hdr.signature[0] != 0x55 || part_hdr.signature[1] != 0xAA) {
        printf("file header has signature 0x%02X,0x%02X instead of 0x55,0xAA\n", part_hdr.signature[0], part_hdr.signature[1]);
        fclose(fp);
        return -1;
    }

    // read the store header
    wmstore_hdr_t store_hdr;
    if (fread(&store_hdr, sizeof(store_hdr), 1, fp) != 1) {
        printf("failed to read wmstore header\n");
        fclose(fp);
        return -1;
    }
    // verify magic
    if (memcmp(store_hdr.magic, "_wmstore", 8) != 0) {
        printf("invalid store magic (expected _wmstore)\n");
        fclose(fp);
        return -1;
    }

    // print info about the store
    printf("WMSTORE:\n");
    printf("  Name: %s\n", cheap_wchar_to_ascii(store_hdr.name));
    printf("  GUID: ");
    print_guid(store_hdr.guid);
    printf("  Unk1: 0x%x\n", store_hdr.unk1);
    printf("  Unk2: 0x%x\n", store_hdr.unk2);
    printf("  Unk3: 0x%x\n", store_hdr.unk3);
    printf("  Timestamp: TODO\n"); // store_hdr.timestamp
    printf("  Unk5: 0x%x\n", store_hdr.unk5);

    // read partitions - there isn't a count afaict so just do it until we stop seeing partitions
    int parts = 0;
    while (true)
    {
        wmpart_hdr_t part_hdr;
        if (fread(&part_hdr, sizeof(part_hdr), 1, fp) != 1) {
            printf("failed to read wmpart header\n");
            fclose(fp);
            return -1;
        }
        if (memcmp(part_hdr.magic, "_wmpart_", 8) != 0) {
            break;
        }

        // print info about the partition
        printf("WMPART %i:\n", parts);
        printf("  Name: %s\n", cheap_wchar_to_ascii(part_hdr.name));
        printf("  Unk1: 0x%x\n", part_hdr.unk1);
        printf("  Offset: 0x%x (@ 0x%x)\n", part_hdr.offset_sector, part_hdr.offset_sector * SECTOR_SIZE);
        printf("  Unk2: 0x%x\n", part_hdr.unk2);
        printf("  Size: 0x%x (0x%x)\n", part_hdr.size_sectors, part_hdr.size_sectors * SECTOR_SIZE);
        printf("  Unk3: 0x%x\n", part_hdr.unk3);
        printf("  Timestamp: TODO\n"); // part_hdr.timestamp
        printf("  Partition Type: 0x%x\n", part_hdr.partition_type);
        printf("  Unk5: 0x%x\n", part_hdr.unk5);
        printf("  Unk6: 0x%x\n", part_hdr.unk6);
        parts++;
    }

    fclose(fp);

    return 0;
}
