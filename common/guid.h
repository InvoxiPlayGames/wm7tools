#ifndef _GUID_H_
#define _GUID_H_

#include <stdint.h>

typedef struct _guid {
    uint32_t p1;
    uint16_t p2;
    uint16_t p3;
    uint16_t p4;
    uint8_t p5[6];
} guid_t;

#endif // _GUID_H_
