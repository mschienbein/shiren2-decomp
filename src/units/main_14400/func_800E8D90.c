#include "common.h"
typedef unsigned char u8;
typedef struct Obj800E8A68 Obj800E8A68;
typedef struct S S;
extern void *func_800E8A68(Obj800E8A68 *, u8);
extern s32 func_8010CD1C(S *);
static inline u32 item_value(S *item) {
    u32 result;
    if (!item) result = 0;
    else result = (unsigned short)func_8010CD1C(item);
    return result;
}
u32 func_800E8D90(Obj800E8A68 *object) {
    return item_value(func_800E8A68(object, 4));
}
