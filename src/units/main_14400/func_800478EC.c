#include "common.h"

typedef struct {
    unsigned char unk00[0x10];
    unsigned char unk10[0x14];
    signed char unk24[5];
} Object;
extern void func_80048764(void *);
extern void func_80048870(void *, s32);
extern void func_800487EC(void *, s32, s32, char *);
extern char *func_80112A6C(unsigned char, s32);
extern s32 func_800327C0(char *, const char *, ...);
typedef struct {
    unsigned char widths[8];
    unsigned char heights[8];
} StatusTables;
extern const StatusTables D_80138C60;
extern char D_8014A9D0[];

void func_800478EC(Object *object) {
    char text[80];
    s32 index;
    s32 code;
    signed char value;
    void *part = object->unk10;
    func_80048764(part);
    func_80048870(part, 0x78000000);
    index = 0;
    code = -0x17;
    for (;;) {
        if (index < 5) {
            value = object->unk24[index];
            if (value >= 0) {
                /* Both byte indexes stay inside the complete table allocation. */
                func_800327C0(text, D_8014A9D0, ((const unsigned char *)&D_80138C60)[code + 0x1F], ((const unsigned char *)&D_80138C60)[code + 0x17], func_80112A6C((unsigned char)code, value + 1));
                func_800487EC(object->unk10, index, 0, text);
            }
            code++;
            index++;
        } else {
            return;
        }
    }
}
