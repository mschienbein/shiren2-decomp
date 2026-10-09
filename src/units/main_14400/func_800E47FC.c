#include "common.h"

typedef struct Object Object;
struct Object {
    unsigned char unk00[9];
    unsigned char unk09;
    unsigned char unk0A[0x12];
    unsigned short unk1C;
    unsigned char unk1E;
    unsigned char unk1F[0x39];
    Object *unk58; /* current target object (func_800A6420/func_800A22B8 operand) */
    unsigned char unk5C[0x16];
    unsigned char unk72;
    unsigned char unk73[0x28];
    unsigned char unk9B;
};
extern s32 func_800E2074(Object *);
extern s32 func_800A6FD0(Object *);
extern u32 func_800B1C6C(Object *);
extern s32 func_800E1D14(Object *, s32);
extern s32 func_800E1CC4(Object *, s32);
extern unsigned char func_800A6420(Object *, Object *);
extern unsigned char *func_800A22B8(unsigned char *, Object *, Object *);
extern unsigned char *func_800A65E4(unsigned char *, Object *, Object *);
extern Object *D_801476B8;

s32 func_800E47FC(Object *object) {
    unsigned char direction;
    unsigned char fallback;
    s32 blocked = 0;
    s32 available;
    if (func_800E2074(object) == 0 || func_800A6FD0(object) != 0 ||
        (object->unk1C & 0x200) || (object->unk72 & 2) ||
        func_800E1D14(object, 0x13) != 0 || func_800E1CC4(object, 0) != 0 ||
        func_800E1D14(object, 0x14) != 0 ||
        ((object->unk09 & 0xF) == 1 && (func_800B1C6C(object) & 0x80))) {
        blocked = 1;
    }
    if (blocked != 0) {
        return -1;
    }
    if ((func_800A6420(object, object->unk58) & 0xFF) != 3) {
        func_800A22B8(&direction, object, object->unk58);
        if (func_800E1CC4(object, 4) != 0) {
            direction = (direction + 4) & 7;
        }
        return direction;
    }
    available = 0;
    if (((object->unk1E >> 4) & 1) && (object->unk9B >> 7)) {
        s32 status = func_800A6420(object, D_801476B8) & 0xFF;
        if (status != 3) {
            available = 1;
        } else {
            available = 0;
        }
    }
    if (available != 0) {
        func_800A65E4(&fallback, object, D_801476B8);
        return fallback;
    }
    return -1;
}
