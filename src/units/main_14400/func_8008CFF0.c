#include "common.h"
typedef struct { unsigned char pad[0x4C0]; s32 field4C0; } Object;
extern s32 func_8008C908(void *);
extern void func_8008CA60(void *);
void func_8008CFF0(Object *p) {
    u32 i = 0;
    unsigned char *entry = (unsigned char *)p;
    s32 offset;
    for (; i < 8; i++, entry += 0x20) {
        if (func_8008C908(entry)) p->field4C0--;
    }
    for (i = 0, offset = 0x100; i < 8; i++, offset += 0x78) {
        unsigned char *item = (unsigned char *)p + offset;
        if (*item) func_8008CA60(item);
    }
}
