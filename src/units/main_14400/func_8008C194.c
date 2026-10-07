#include "common.h"

typedef unsigned char u8;
/* Opaque view of the allocation described by struct Manager in func_8008D2A8.c. */
typedef struct Manager Manager;

extern s32 D_8013FEE0;
extern u8 *D_8013FEE4;

void func_8008D2A8(Manager *mgr, s32 index, s32 force);

void func_8008C194(s32 arg0) {
    if (D_8013FEE0 != 0) {
        func_8008D2A8((Manager *)D_8013FEE4, arg0, 0);
    }
}
