#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* Partial view: index at 0x54 and complete 0x10-byte text box at 0x1C4. */
typedef struct Obj8009F194 {
    u8 pad_00[0x54];
    s32 index_54;
    u8 pad_58[0x1C4 - 0x58];
    u8 text_1C4[0x10];
} Obj8009F194;

extern u16 D_80154258[]; /* message id per index */
extern u8 D_801428B4[];  /* text width per index */

char *func_80048480(u16 id);
s32 func_8005EF30(char *dst, const char *fmt, ...);
void func_800487EC(void *text, s32 style, s32 flags, char *str);

void func_8009F194(Obj8009F194 *obj)
{
    char buf[32];
    char *fmt = func_80048480(0x52C);
    char *name = func_80048480(D_80154258[obj->index_54]);

    func_8005EF30(buf, fmt, (0x86 - D_801428B4[obj->index_54]) / 2, name);
    func_800487EC(obj->text_1C4, 0, 0, buf);
}
