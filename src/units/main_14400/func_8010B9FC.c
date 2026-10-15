#include "common.h"
#include "item_records.h"

typedef unsigned char u8;
typedef short s16;

extern char D_8015CDCC[];
s32 func_8010B9F4(Item *obj);
char *func_800AC990(void *obj);
s32 func_8005EF08(char *dst, const char *fmt, ...);
char *func_80083C90(char *dst, char *src);
char *func_8010B9FC(Item *obj, char *buf) {
    s16 bonus = func_8010B9F4(obj);
    if ((obj->flags02 & 2) && bonus != 0) {
        func_8005EF08(buf, D_8015CDCC, func_800AC990(obj), bonus);
        return buf;
    }
    func_80083C90(buf, func_800AC990(obj));
    return buf;
}
