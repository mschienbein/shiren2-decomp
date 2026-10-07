#include "common.h"

typedef unsigned char u8;
typedef struct { char pad0[0x5C]; u8 *unk5C; char pad60[0x1C]; s32 unk7C; } Obj;
void func_80097240(void *n, void *record, void *pairs, void *counts);
void func_8009A600(Obj *obj, u8 *data, void *record, void *pairs, void *counts) {
    func_80097240(obj, record, pairs, counts);
    if (data == 0) {
        obj->unk5C = 0;
    } else {
        switch (*data) {
            case 3:
            case 4:
                obj->unk5C = data;
                break;
            default:
                obj->unk5C = 0;
                break;
        }
    }
    obj->unk7C = 0;
}
