#include "common.h"

typedef unsigned short u16;

/* 0x74-byte task record (D_801BA380 table, see func_80084CD4). */
typedef struct {
    unsigned char pad_0[4];
    u16 field_4;
    unsigned char pad_6[0x74 - 0x06];
} Entry;

/* Partial view of the waiting task: state at +8, target task index at +0x10. */
typedef struct {
    unsigned char pad_0[4];
    u16 field_4;
    u16 field_6;
    u16 field_8;
    unsigned char pad_A[4];
    u16 field_E;
    u16 field_10;
} Obj;

extern Entry D_801BA380[];

/* Task handler (TaskFn: void (*)(void *)), registered by func_8004A558. */
void func_800853D4(void *task)
{
    Obj *obj = task;

    switch (obj->field_8) {
    case 0:
        if (obj->field_10 == obj->field_6) {
            obj->field_E = 1;
            obj->field_4 = 4;
            break;
        }
        obj->field_8++;
        /* fall through */
    case 1:
        if (D_801BA380[obj->field_10].field_4 == 4) {
            obj->field_E = 1;
            obj->field_4 = 4;
        }
        break;
    }
}
