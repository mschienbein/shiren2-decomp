#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x54];
    s32 field_54;
    u8 ids_58[0xFC - 0x58];
    s32 field_FC;
} Obj8009F794;

extern char *func_80048480(u16 id);
extern char *func_80083C90(char *dst, char *src);
extern void func_8009F818(void *owner, s32 id, char *out);

void func_8009F794(Obj8009F794 *obj, s32 index, char *out) {
    if (obj->field_54 != 0) {
        if (obj->ids_58[index] != 0) {
            func_8009F818(obj, index, out);
        } else {
            func_80083C90(out, func_80048480(0x290));
        }
    } else if (obj->field_FC == 0) {
        if (index == 0) {
            func_80083C90(out, func_80048480(0x252));
        } else {
            *out = 0;
        }
    } else {
        func_8009F818(obj, obj->ids_58[index], out);
    }
}
