#include "common.h"
#include "item_records.h"

typedef unsigned char u8;
typedef short s16;
typedef struct { char pad0[0x1E]; u8 unk1E; } Ent;
typedef struct { s32 type; Ent *ent; } Msg;
extern short D_801569CA;
extern short D_801569CC;
s32 func_800E9270(Ent *, Item *, s32, s32);
void func_800EB598(Ent *obj, s16 amount, s16 alternate_amount);
void func_80112EAC(Item *);
s32 func_801131F8(Item *, Msg *);
s32 func_8011CAB0(Item *self, Msg *msg) {
    if (msg->type == 3) {
        Ent *ent = msg->ent;
        if ((ent->unk1E >> 2) & 1) {
            if (func_800E9270(ent, self, 0x17, 0) != 0) {
                func_800EB598(ent, D_801569CA, D_801569CC);
                func_80112EAC(self);
                if (self != 0) {
                    self->field08->method0C((char *)self + self->field08->delta08, 3);
                }
                return 1;
            }
            return 0;
        }
        return 0;
    }
    return func_801131F8(self, msg);
}
