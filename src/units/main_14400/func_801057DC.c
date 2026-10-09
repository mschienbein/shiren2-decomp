#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad_0[0x54]; u8 field_54; u8 pad_55[0x89 - 0x55]; u8 field_89; } Object;
typedef struct { s32 type; } Message;
extern u32 func_800B1C6C(Object *pos);
extern s32 func_800E1CC4(Object *obj, s32 kind);
extern u16 func_800E08B0(void *obj);
extern s32 func_800E0534(void *obj, s32 amount);
extern s32 func_800F27A4(Object *obj, Message *msg);
extern u32 D_8013960C;

/* ODD_C: boolean helper; GCC 2.8.1 expands its result as sne (sltu rd,$zero,rs), as the original does. */
static inline u8 nonzero(u32 value)
{
    return value != 0;
}

s32 func_801057DC(Object *object, Message *message)
{
    u32 active;
    if (message->type == 2) {
        active = 0;
        if ((func_800B1C6C(object) & 0x2000) && !(object->field_54 & 2) && func_800E1CC4(object, 2) == 0) {
            active = nonzero(func_800E08B0(object));
        }
        if (active != 0) {
            D_8013960C *= 2;
            func_800E0534(object, object->field_89);
            D_8013960C >>= 1;
        }
    }
    return func_800F27A4(object, message);
}
