#include "common.h"
typedef struct { void (*field0)(void *); unsigned char pad4[0x58]; s32 field5C; unsigned char pad60[8]; s32 field68; unsigned char pad6C[8]; } Entry;
extern Entry D_801BA380[];
extern void func_8008B558(void *), func_8008B678(void *);
extern unsigned short func_80084BCC(void);
void func_8008B68C(s32 first, s32 second) {
    s32 i = func_80084BCC() - 1;
    for (; i >= 0; i--) {
        Entry *entry = &D_801BA380[i];
        if (entry->field0 == func_8008B558 && entry->field5C == first && entry->field68 == second) {
            entry->field0 = func_8008B678;
            break;
        }
    }
}
