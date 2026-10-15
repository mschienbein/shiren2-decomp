#include "common.h"
#include "item_records.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

char *func_800A8498(s32 id, s32 variant);
char *func_80083C90(char *dst, char *src);
char *func_80128560(Item *obj, char *dst) {
    /* D_80160720 is installed by the id-0xF1 constructor func_80128280. */
    func_80083C90(dst, func_800A8498(obj->payload.containerF1.field0D, obj->payload.containerF1.field0E));
    return dst;
}
