#include "common.h"
typedef struct { unsigned char field0, field1; unsigned char pad2[0xA]; unsigned char fieldC; } Object;
extern char *func_80048480(unsigned short id);
extern s32 func_8005EF08(char *dst, const char *fmt, ...);
extern char *func_80083C90(char *dst, char *src);
extern char *func_800AC990(void *obj);
extern char *func_800ACB40(void *obj);
extern s32 func_800AD468(u32 bit);
extern unsigned short func_80112DC4(unsigned char);
void *func_80113100(Object *p, void *out) {
    if (p->fieldC & 1) {
        func_80083C90(out, func_80048480(0x22E));
    } else {
        char *value = func_800ACB40(p);
        s32 result = func_800AD468(p->field1) ^ 1;
        if (result && value) {
            func_8005EF08(out, func_80048480(0x2A1), value);
        } else if ((p->fieldC >> 1) & 1) {
            char *format = func_80048480(0x24B);
            func_8005EF08(out, format, func_80048480(func_80112DC4(p->field1)));
        } else {
            func_80083C90(out, func_800AC990(p));
        }
    }
    return out;
}
