#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { s32 unk0; s32 unk4; } Pair;
typedef struct { s32 unk0; s32 unk4; s32 key; } Entry;
typedef struct { u8 pad[0x3C]; Pair unk3C; } S;
u8 *func_8006A810(void *dst, s32 value, s32 count);
void func_80097240(S *, Entry *, char *, Pair *);
extern Entry D_801428C0[];
extern char D_80139094[];
void func_8009FDD0(S *arg0, s32 key) {
    Pair args;
    s32 i;

    func_8006A810(&args, 0, sizeof(args));
    args.unk0 = 3;
    func_80097240(arg0, D_801428C0, D_80139094, &args);
    i = 0;
    while (1) {
        if (i >= 3) {
            break;
        }
        if (D_801428C0[i].key == key) {
            Pair result;
            func_8006A810(&result, 0, sizeof(result));
            result.unk0 = i;
            arg0->unk3C = result;
        }
        i++;
    }
}
