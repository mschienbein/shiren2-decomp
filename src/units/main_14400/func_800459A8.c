#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s8 x; s8 y; } Pair800459A8;
typedef struct { s32 w[6]; } Buf800459A8;
extern Pair800459A8 D_8014BF98;
void func_80045370(Buf800459A8 *buf, s32 arg1, Pair800459A8 pos);
void func_800453B8(Pair800459A8 *pos, Buf800459A8 *buf);
void func_80045948(void *obj, Pair800459A8 pos);
void func_800459A8(void *obj, s32 arg1) {
    Buf800459A8 buf;
    Pair800459A8 pos;
    pos = D_8014BF98;
    func_80045370(&buf, arg1, pos);
    func_800453B8(&pos, &buf);
    func_80045948(obj, pos);
}
