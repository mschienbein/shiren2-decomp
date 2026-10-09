#include "common.h"

extern signed char D_80154264[];
signed char func_800CB510(unsigned char key) {
    signed char *entry = D_80154264;
    while (*entry != -1) {
        unsigned char current = *entry;
        if ((signed char)current == key) return entry[1];
        entry += 2;
    }
    return -1;
}
