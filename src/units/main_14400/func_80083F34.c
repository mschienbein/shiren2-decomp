#include "common.h"
extern unsigned char *func_80083EE4(unsigned char ch, unsigned char *dst);
char *func_80083F34(unsigned char *cursor, s32 length, char *destination) {
    if (cursor) {
        while (*cursor && length-- > 0) destination = (char *)func_80083EE4(*cursor++, (unsigned char *)destination);
    }
    return destination;
}
