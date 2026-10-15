#include "common.h"
#include "item_records.h"

typedef unsigned char u8;
extern char *func_800A8498(s32 id, s32 arg);
extern char *func_800ACC30(u8 arg0);
extern char *func_80083C90(char *dst, char *src);

char *func_80128CAC(Item *obj, char *dst) {
    char *src;
    /* D_80160770 is installed by the id-0xF2 constructor func_80128C40. */
    if (obj->payload.appearance.flags0D >> 7) {
        src = func_800A8498(0x29, obj->payload.appearance.flags0D & 0x7F);
    } else {
        src = func_800ACC30(obj->payload.appearance.field0C);
    }
    func_80083C90(dst, src);
    return dst;
}
