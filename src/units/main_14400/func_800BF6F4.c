#include "common.h"
typedef unsigned short u16;
typedef struct { unsigned char pad[0x958]; u16 field958; } Object;
extern void func_800BF2F4(Object *);
extern void func_800BF5C4(Object *);
s32 func_800BF6F4(Object *obj) {
    if (!(obj->field958 & 0x800)) return 0;
    func_800BF2F4(obj);
    func_800BF5C4(obj);
    return 1;
}
