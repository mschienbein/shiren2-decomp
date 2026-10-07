#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

typedef struct { s32 a, b; } Entry;
extern s32 D_801CA714;
extern s32 D_801CA718;
extern s32 D_801CA71C;
extern Entry *D_801CA720;
extern void func_8012ABAC(Entry *);
void func_8012AB24(void){
    while (D_801CA714 != D_801CA718) {
        func_8012ABAC(&D_801CA720[D_801CA714]);
        if (++D_801CA714 == D_801CA71C) D_801CA714 = 0;
    }
}
