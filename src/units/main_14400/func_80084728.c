#include "common.h"
typedef unsigned short u16;
typedef struct { char pad_00[4]; u16 field_04; u16 pad_06; u16 state_08; char pad_0A[8]; u16 flags_12; s32 field_14; s32 field_18; s32 timer_1C; } Object;
extern s32 D_8013E924;
extern s32 func_80054920(void);
extern void func_8005422C(s32 mode);
extern s32 func_8006D550(s32 mode);
extern s32 func_80041FF8(void);
extern void func_8005484C(void);
extern void func_8005493C(void);
void func_80084728(Object *object) {
    switch (object->state_08) {
    case 0:
        if (func_80054920() != 0) break;
        func_8005422C(1);
        if (object->flags_12 & 0x1000) {
            object->timer_1C = 20;
            object->state_08 = 2;
            break;
        }
        object->state_08++;
        /* fall through */
    case 1:
        if (func_8006D550(6) == 1 || (func_80041FF8() & 0xFF)) {
            func_8005484C();
            func_8005422C(0);
            if (object->field_14 == 0) func_8005493C();
            D_8013E924 = 1;
            object->field_04 = 4;
        }
        break;
    case 2:
        if (object->timer_1C-- == 0) {
            func_8005484C();
            func_8005422C(0);
            if (object->field_14 == 0) func_8005493C();
            object->field_04 = 4;
        }
        break;
    }
}
