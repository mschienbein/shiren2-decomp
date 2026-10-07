#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { u8 pad0[0x58]; void *target_58; u8 pad5C[0x2D]; u8 level_89; } Obj800FC540;
s32 func_800A692C(Obj800FC540 *obj, s32 kind);
void *func_800F18A4(Obj800FC540 *obj, u8 level, s32 arg);
s32 func_800A65B8(void *obj, void *target);
u32 func_800B1C6C(void *target);
s32 func_800A6E90(void *target);
s32 func_800F1024(Obj800FC540 *obj);
void func_800F06E4(Obj800FC540 *obj);
s32 func_800E1CD4(Obj800FC540 *obj, s32 kind);
s32 func_800E7104(Obj800FC540 *obj);
s32 func_800E8350(Obj800FC540 *obj);

s32 func_800FC540(Obj800FC540 *obj) {
    if ((func_800A692C(obj, 0x12) ^ 1) != 0) {
        void *target = obj->target_58;
        s32 ok;

        if (target == 0) {
            target = func_800F18A4(obj, obj->level_89, 1);
            obj->target_58 = target;
        }
        ok = 0;
        if (func_800A65B8(obj, target) <= obj->level_89 && !(func_800B1C6C(target) & 0x4000)) {
            ok = func_800A6E90(target) == 0;
        }
        if (ok && func_800F1024(obj)) {
            func_800F06E4(obj);
            return 0;
        }
    }
    if (func_800E1CD4(obj, 0x10)) {
        return func_800E8350(obj);
    }
    return func_800E7104(obj);
}
