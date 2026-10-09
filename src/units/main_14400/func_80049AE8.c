#include "common.h"
typedef struct { unsigned char field_00[0x12]; unsigned short field_12; } Object;
extern s32 D_8013960C;
/* Whole menu-system object; its signed display-mode byte is at +4. */
extern signed char D_80140160[];
extern char D_80139698[];
extern s32 func_80046240(void);
extern char *func_80048480(unsigned short message);
extern void func_800265E0(void *buffer, s32 size);
extern s32 func_8005ECF8(char *buffer, const char *format, char *args);
extern void func_800492E4(char *buffer);
extern unsigned int func_80032D70(const char *buffer);
extern unsigned int func_80048AC8(void);
extern void func_80084A68(void);
extern s32 func_80083FF8(void);
extern Object *func_80084220(char *buffer, s32 delay);
void func_80049AE8(s32 message, ...)
{
    char *args;
    char *format;
    Object *obj;
    s32 toggled, state;
    if (func_80046240() || !(D_8013960C & 1)) return;
    /* local-arithmetic-qualification: GCC 2.8.1 va-mips.h o32 va_arg aligns the
     * ABI argument save area; this is not a game-object address. */
    args = (char *)(((u32)__builtin_next_arg(message) + 3) & ~3U) + 4;
    if (*(s32 *)(args - 4) == -2) return;
    format = func_80048480(message);
    func_800265E0(D_80139698, 255);
    /* The formatter's byte count and the string length are intentionally unused. */
    func_8005ECF8(D_80139698, format, args);
    func_800492E4(D_80139698);
    func_80032D70(D_80139698);
    if (func_80048AC8()) {
        func_80084A68();
        obj = func_80084220(D_80139698, func_80083FF8());
        state = D_80140160[4];
        toggled = state ^ 1;
        if (toggled) return;
        obj->field_12 |= 0x1000;
    }
}
