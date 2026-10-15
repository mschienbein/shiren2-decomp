#include "common.h"
#include "item_records.h"
extern char *func_80048480(unsigned short id);
extern s32 func_8005EF08(char *dst, const char *fmt, ...);
extern char *func_80083C90(char *dst, char *src);
extern char *func_800AC990(void *obj);
extern char *func_800ACB40(void *obj);
extern s32 func_800AD468(u32 bit);
extern unsigned short func_80112DC4(unsigned char);
char *func_80113100(Item *p, char *out) {
    /* D_8015D730 is installed by the kind-2 constructor func_80112D20. */
    if (p->payload.consumable.flags0C & 1) {
        func_80083C90(out, func_80048480(0x22E));
    } else {
        char *value = func_800ACB40(p);
        s32 result = func_800AD468(p->field01) ^ 1;
        if (result && value) {
            func_8005EF08(out, func_80048480(0x2A1), value);
        } else if ((p->payload.consumable.flags0C >> 1) & 1) {
            char *format = func_80048480(0x24B);
            func_8005EF08(out, format, func_80048480(func_80112DC4(p->field01)));
        } else {
            func_80083C90(out, func_800AC990(p));
        }
    }
    return out;
}
