#include "common.h"

typedef unsigned short u16;
typedef char *va_list;
typedef struct { unsigned char pad00[0x12]; u16 flags12; } Object;
extern u16 D_801F5D0E;
extern s32 D_8013960C;
extern char D_80139698[];
/* Whole menu-system object; its signed display-mode byte is at +4. */
extern signed char D_80140160[];
extern char *func_80048480(u16 id);
extern void func_800265E0(void *dst, s32 size);
extern s32 func_8005ECF8(char *dst, const char *fmt, va_list args);
extern void func_800492E4(void *object);
extern u32 func_80048AC8(void);
extern void func_80084A68(void);
extern Object *func_80084220(void *text, s32 mode);

static inline s32 display_mode(void)
{
    return D_80140160[4];
}

void func_800497F0(s32 id, ...)
{
    if (D_801F5D0E == 0 && (D_8013960C & 1)) {
        va_list args = (va_list)__builtin_next_arg(id);
        s32 mode;
        const char *fmt;
        Object *object;
        /* local-arithmetic-qualification: GCC 2.8.1 va-mips.h o32 va_arg
         * aligns the ABI argument save area; this is not a game-object address. */
        args = (char *)(((s32)args + 3) & -4) + 4;
        mode = *(s32 *)(args - 4);
        if (mode != -2) {
            fmt = func_80048480(id);
            func_800265E0(D_80139698, 0xFF);
            func_8005ECF8(D_80139698, fmt, args);
            func_800492E4(D_80139698);
            if (func_80048AC8()) {
                func_80084A68();
                object = func_80084220(D_80139698, 0);
                if ((display_mode() ^ 1) == 0)
                    object->flags12 |= 0x1000;
            }
        }
    }
}
