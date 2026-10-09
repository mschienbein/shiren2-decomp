#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad00[0x54];
    u8 flags54;
    u8 pad55[0x89 - 0x55];
    u8 field89;
} Obj_800F96B4;

typedef struct {
    s32 kind;
} Msg_800F96B4;

extern s32 func_800F27A4(Obj_800F96B4 *obj, Msg_800F96B4 *msg);
extern u32 func_800B1C6C(Obj_800F96B4 *pos);
extern s32 func_800E1CC4(Obj_800F96B4 *obj, s32 kind);
extern u16 func_800E08B0(Obj_800F96B4 *obj);
extern s32 func_800E0534(Obj_800F96B4 *obj, s32 amount);
extern u32 D_8013960C;

/* ODD_C: boolean helper; GCC 2.8.1 expands its result as sne (sltu rd,$zero,rs), as the original does. */
static inline u8 nonzero(u32 value)
{
    return value != 0;
}

s32 func_801064A0(Obj_800F96B4 *self, Msg_800F96B4 *msg)
{
    if (msg->kind == 2) {
        s32 boost = 0;

        if ((func_800B1C6C(self) & 0x2000) && !(self->flags54 & 2) && !func_800E1CC4(self, 2)) {
            boost = nonzero(func_800E08B0(self));
        }
        if (boost) {
            D_8013960C <<= 1;
            func_800E0534(self, self->field89);
            D_8013960C >>= 1;
        }
    }
    return func_800F27A4(self, msg);
}
