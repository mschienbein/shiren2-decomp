#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s32 row; s32 col; } Pos800D4D8C;
/* Pool container: pool-record pointer at +0 (consumed by func_800D4A28), busy flag at +8. */
typedef struct { void *pool; u8 pad4[0x4]; s8 busy; } Obj800D4D8C;
s32 func_800D4A10(Obj800D4D8C *obj);
Pos800D4D8C *func_800D4B60(Pos800D4D8C *pos, Obj800D4D8C *obj, s32 index);
void *func_800B4E18(Pos800D4D8C *pos);
void func_800D4A28(void **pool, s32 index, void *value);
void func_800D4C34(Obj800D4D8C *obj);
void func_800D4D8C(Obj800D4D8C *obj) {
    s32 i;
    if (obj->busy == 0) {
        i = func_800D4A10(obj) - 1;
        while (1) {
            Pos800D4D8C pos;
            if (i < 0) {
                break;
            }
            func_800D4B60(&pos, obj, i);
            func_800D4A28(&obj->pool, i, func_800B4E18(&pos));
            i--;
        }
        func_800D4C34(obj);
    }
}
