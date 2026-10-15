#include "common.h"
#include "item_records.h"
typedef unsigned short u16;
char *func_80121D80(void *obj, char *buffer);
char *func_80048480(u16 id);
char *func_80083D04(char *dst, char *src);
s32 func_800CD278(void *);
char *func_800AC990(void *obj);
char *func_80083C90(char *dst, char *src);
s32 func_8005EF08(char *dst, const char *fmt, ...);
char *func_80121F58(Item *o, char *out){
    /* Id 0xAC pot constructor func_801216D0 installs D_8015F938. */
    if (func_80121D80(o, out)) {
        ItemList *s = &o->payload.pot.list;
        func_80083D04(out, func_80048480(s->methods->count24((char *)s + s->methods->delta20) ? 0xC6 : 0xC7));
    } else if (func_800CD278(&o->payload.pot.list)) {
        func_80083C90(out, func_800AC990(o));
    } else {
        char *msg = func_80048480(0x247);
        func_8005EF08(out, msg, func_800AC990(o), 0);
    }
    return out;
}
