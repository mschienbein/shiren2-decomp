#include "common.h"

typedef unsigned char u8;
typedef struct {
    u8 pad_00[0xC];
    u8 field_0C;
    u8 field_0D;
} Obj;
extern char *func_800A8498(s32 id, s32 arg);
extern char *func_800ACC30(u8 arg0);
extern char *func_80083C90(char *dst, char *src);

char *func_80128CAC(Obj *obj, char *dst) {
    char *src;
    if (obj->field_0D >> 7) {
        src = func_800A8498(0x29, obj->field_0D & 0x7F);
    } else {
        src = func_800ACC30(obj->field_0C);
    }
    func_80083C90(dst, src);
    return dst;
}
