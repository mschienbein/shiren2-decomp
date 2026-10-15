#include "common.h"
#include "item_records.h"

typedef unsigned short u16;

char *func_80048480(u16 id);
u16 func_800AE710(void *obj);
char *func_800AC990(void *obj);
s32 func_8005EF08(char *dst, const char *fmt, ...);

char *func_801155AC(Item *obj, char *dst) {
    char *name = func_80048480(0x241);
    s32 kind = func_800AE710(obj) & 0xFFFF;

    func_8005EF08(dst, name, kind, func_800AC990(obj));
    return dst;
}
