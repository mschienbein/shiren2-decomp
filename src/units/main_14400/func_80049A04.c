#include "common.h"

typedef unsigned short u16;
typedef char *va_list;
typedef struct { char pad[0x12]; u16 flags; } Obj;
extern u32 D_8013960C;
extern char D_80139698[];
/* Whole menu-system object; its signed display-mode byte is at +4. */
extern signed char D_80140160[];
s32 func_80046240(void);
char *func_80048480(u16);
void func_800265E0(void*, s32);
s32 func_8005ECF8(char *dst, const char *fmt, va_list args);
void func_800492E4(char*);
u32 func_80048AC8(void);
void func_80084A68(void);
s32 func_80083FF8(void);
Obj *func_80084220(char*, s32);
void func_80049A04(u16 id, ...){
    va_list args;
    char *fmt;
    Obj *obj;
    s32 notOne;
    if (func_80046240() == 0 && (D_8013960C & 1)) {
        args = (va_list)__builtin_next_arg(id);
        fmt = func_80048480(id);
        func_800265E0(D_80139698, 0xFF);
        func_8005ECF8(D_80139698, fmt, args);
        func_800492E4(D_80139698);
        if (func_80048AC8()) {
            func_80084A68();
            obj = func_80084220(D_80139698, func_80083FF8());
            notOne = D_80140160[4] != 1;
            if (!notOne) obj->flags |= 0x1000;
        }
    }
}
