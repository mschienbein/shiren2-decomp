#include "common.h"
typedef char *va_list;
typedef struct { unsigned char pad[0x12]; unsigned short field12; } Object;
extern s32 D_8013960C;
extern char D_80139698[];
extern signed char D_80140164;
extern s32 func_80046240(void);
extern char *func_80048480(unsigned short id);
extern void func_800265E0(void *, s32);
extern s32 func_8005ECF8(char *dst, const char *fmt, va_list args);
extern void func_800492E4(void *);
extern u32 func_80048AC8(void);
extern void func_80084A68(void);
extern Object *func_80084220(void *, s32);
static inline s32 display_mode(void) { return D_80140164; }
void func_800498E4(s32 id, ...) {
    if (!func_80046240() && (D_8013960C & 1)) {
        va_list args = (va_list)__builtin_next_arg(id);
        const char *fmt = func_80048480(id);
        func_800265E0(D_80139698, 0xFF);
        func_8005ECF8(D_80139698, fmt, args);
        func_800492E4(D_80139698);
        if (func_80048AC8()) {
            Object *p;
            func_80084A68();
            p = func_80084220(D_80139698, 0);
            if ((display_mode() ^ 1) == 0) p->field12 |= 0x1000;
        }
    }
}
