#include "common.h"

typedef struct {
    unsigned char kind;
    unsigned char id;
    char pad2[0xB];
    unsigned char flags_D;
    char padE[3];
    unsigned char field_11;
} Obj;

extern unsigned char *D_8015380C[];
void func_800ACF34(Obj *obj);
unsigned char func_800AE98C(Obj *obj);
void func_800B0B10(s32 value);

void func_800ACD34(Obj *obj) {
    unsigned char *table;
    unsigned char *slot;

    func_800ACF34(obj);
    table = D_8015380C[obj->kind];
    if (table != 0) {
        slot = table + func_800AE98C(obj);
        func_800B0B10(*slot);
        *slot = 0;
    }
    if (obj->id == 0x99) {
        obj->field_11 = 1;
    } else if (obj->id == 0x98) {
        obj->field_11 = 1;
    } else if (obj->id == 0xF2) {
        obj->flags_D |= 0x80;
    }
}
