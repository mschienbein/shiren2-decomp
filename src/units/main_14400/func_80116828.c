#include "common.h"

typedef union {
    s32 words[2];
    struct {
        unsigned char pad0[3];
        unsigned char x;
        unsigned char pad4[3];
        unsigned char y;
    } bytes;
} Pos;

s32 D_801486C0 = 0;
extern Pos D_801486C4;
extern unsigned char D_801486E8[];
extern unsigned char D_80148718;
extern unsigned char D_80148719;

unsigned char *func_80116720(void);
void func_80116784(void);

void func_80116828(Pos *pos)
{
    unsigned char *entry;

    if (D_801486C0 == 0) {
        entry = D_801486E8;
        D_801486C4 = *pos;
        D_80148719 = 0;
        D_80148718 = 0;
    } else {
        entry = func_80116720();
    }
    if (entry != 0) {
        entry[1] = pos->bytes.y;
        entry[0] = pos->bytes.x;
    }
    if (D_801486C0 == 0) {
        D_801486C0 = 1;
        func_80116784();
        D_801486C0 = 0;
    }
}
