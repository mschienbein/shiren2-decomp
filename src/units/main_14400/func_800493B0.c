#include "common.h"

typedef struct {
    unsigned char bit7 : 1;
    unsigned char state : 3;
    unsigned char bit3 : 1;
    unsigned char bit2 : 1;
    unsigned char low : 2;
} Flags_800493B0;

typedef struct {
    char pad0[0xA];
    unsigned char kind_A;
    char padB[0x1E - 0xB];
    Flags_800493B0 flags_1E;
    unsigned char target_1F;
} Obj_800493B0;

extern s32 func_8007C410(s32, s32);

void func_800493B0(Obj_800493B0 *obj) {

    if (obj->flags_1E.state || obj->flags_1E.bit3 || obj->flags_1E.bit2) {
        u32 target = obj->target_1F;
        if (obj->flags_1E.bit3 || obj->flags_1E.bit2) {
            u32 bit2 = obj->flags_1E.bit2;
            if (bit2 && target == 0x17) {
                func_8007C410(0x17, 0);
            } else if (obj->kind_A == 0x1B && target == 0x1B) {
                func_8007C410(0x1B, 0);
            }
        }
    }
}
