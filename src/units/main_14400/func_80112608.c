#include "common.h"
typedef unsigned short u16;
char *func_80048480(u16 id);
u16 func_800AE710(void *obj);
char *func_800AC990(void *obj);
s32 func_8005EF08(char *dst, const char *fmt, ...);
void *func_80112608(void *obj, void *out) {
    char *text = func_80048480(0x23F);
    s32 value = func_800AE710(obj);
    func_8005EF08(out, text, value, func_800AC990(obj));
    return out;
}
