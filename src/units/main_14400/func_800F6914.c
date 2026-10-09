#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct Buf800D8FB0 Buf800D8FB0;
typedef struct { void *field_00; void *field_04; const char *field_08; s32 field_0C; } Action;
extern unsigned char D_80140160[];
extern void func_800498E4(s32 id, ...);
extern void func_80049BF0(s32 mode);
extern s32 func_80049CB4(s32 id, ...);
extern char *func_80048480(u16 id);
extern s32 func_800327C0(char *dst, const char *fmt, ...);
/* The original caller passes the dialog object address, not an integer token. */
extern s32 func_80094CE8(void *dialog, char *text);
extern Buf800D8FB0 *func_800D8FB0(u32 size);
extern Buf800D8FB0 *func_800D9C80(Buf800D8FB0 *storage, void *target);
Buf800D8FB0 *func_800F6914(Action *action)
{
    char text[64];
    Buf800D8FB0 *result = 0;
    func_800498E4(0x1C9, action->field_08, action->field_0C);
    func_80049BF0(0);
    func_80049CB4(2);
    func_800327C0(text, func_80048480(0x1CB), action->field_08);
    if ((u8)func_80094CE8(D_80140160, text) == 1) {
        func_800498E4(0x1CC, action->field_08);
        result = func_800D9C80(func_800D8FB0(12), action->field_04);
    } else {
        func_800498E4(0x1CD, action->field_08);
    }
    func_80049BF0(0);
    func_80049CB4(2);
    return result;
}
