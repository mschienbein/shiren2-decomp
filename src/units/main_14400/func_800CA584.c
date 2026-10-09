#include "common.h"
unsigned short func_800CA560(unsigned short);
/* The original member-style receiver is supplied but unused by this checksum. */
unsigned short func_800CA584(void *stream, const unsigned char *s) {
    unsigned short crc = 9;
    s32 i;
    while (*s != 0) {
        crc = func_800CA560(crc + *s++);
    }
    for (i = 2; i >= 0; i--) {
        crc = func_800CA560(crc);
    }
    return crc;
}
