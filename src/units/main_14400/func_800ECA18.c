#include "common.h"

typedef struct {
    unsigned char fields_00[0x95];
    unsigned char entries_95[32];
} Object;

void func_800ECA18(Object *object) {
    s32 i;
    for (i = 31; i != -1; --i) {
        object->entries_95[i] = 0;
    }
}
