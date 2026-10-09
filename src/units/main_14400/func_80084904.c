#include "common.h"

typedef unsigned short u16;

/* Saved (mode, current task) pair stack; pushed by func_80084A20, popped by func_80084B80. */
typedef struct {
    u16 first;
    u16 second;
} Pair;

/* 0x74-byte task record (see func_80084CD4). */
typedef struct {
    void (*handler)(void *);
    u16 field04;
    unsigned char pad06[0x74 - 0x06];
} Record;

extern s32 D_8013E900;
extern Pair D_801BF258[];
extern s32 D_801BF2C0;
extern s32 D_801BF2C4;
extern s32 D_801BF2C8;
extern s32 D_801BF2CC;
extern s32 D_8013E908;
extern s32 D_8013E90C;
extern s32 D_8013E914;
extern Record D_801BA380[174];
extern void func_800265E0(void *dst, s32 size);

void func_80084904(void)
{
    s32 i;
    u16 mode = D_8013E900;

    D_801BF2C0 = 0;
    D_801BF2C4 = 0;
    D_8013E908 = 0;
    D_801BF2C8 = 0;
    D_801BF2CC = 0;
    D_8013E914 = 0;
    D_8013E90C = 1;
    D_801BF258[D_801BF2C4].first = mode;
    D_801BF258[D_801BF2C4].second = D_801BF2C0;
    for (i = 0; i < 174; i++) {
        func_800265E0(&D_801BA380[i], sizeof(Record));
        D_801BA380[i].field04 = 0;
    }
}
