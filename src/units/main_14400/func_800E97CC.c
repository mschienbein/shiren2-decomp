#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef struct { u8 pad_00[0x32]; u8 field_32; u8 pad_33[0x45]; u32 field_78; s32 field_7C; } Obj;
extern s32 func_800E0F40(Obj *obj);
extern u32 func_800E9BB4(Obj *obj, u8 level);
extern void func_800E946C(void *obj, s16 count);
extern char *func_800A3B20(Obj *obj);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800498E4(s32 id, ...);
static inline u8 effective_level(Obj *obj)
{
    return func_800E0F40(obj);
}
void func_800E97CC(Obj *obj, s16 amount)
{
    s16 remaining = amount;
    s16 level;
    s16 before;
    s16 after;
    s16 target;
    char *name;
    if ((s16)amount == 0) {
        return;
    }
    before = effective_level(obj);
    target = before + amount;
    level = 0;
    for (;;) {
        s32 pending = remaining != 0;
        if (!pending) {
            break;
        }
        level = obj->field_32 + remaining;
        if (level <= 0) {
            level = 1;
            obj->field_32 = level;
            break;
        }
        if (level >= 100) {
            level = 99;
            obj->field_32 = level;
            break;
        }
        obj->field_32 = level;
        remaining = target - effective_level(obj);
    }
    obj->field_78 = func_800E9BB4(obj, (u8)level);
    func_800E946C(obj, (s16)(effective_level(obj) - before));
    after = effective_level(obj);
    name = func_800A3B20(obj);
    before = after - before;
    if (before != 0) {
        func_80049CB4(0x20, obj, before);
        if (before > 0) {
            func_800498E4(0x25, name, after);
        } else if (before < 0) {
            func_800498E4(0x26, name, after);
        }
        if (before != 0) {
            obj->field_7C = 0;
        }
    }
}
