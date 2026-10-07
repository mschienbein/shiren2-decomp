#include "common.h"

typedef unsigned short u16;

typedef struct { char pad0[0x2FC]; s32 field_2FC; } S;
char *func_80048480(u16 id);
char *func_8009811C(S *s, s32 flag) {
    s32 id;

    switch (s->field_2FC) {
        case 2:
            id = 0x24F;
            break;
        case 4:
        case 8:
        case 0x10:
        case 0x2000:
        case 0x4000:
        case 0xC000:
            id = 0x250;
            break;
        case 0x400:
            if (flag == 0) {
                id = 0x24E;
                break;
            }
            /* fallthrough */
        case 0x200:
            id = 0x251;
            break;
        default:
            id = 0x24E;
            break;
    }
    return func_80048480(id);
}
