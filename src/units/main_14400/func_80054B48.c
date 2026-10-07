#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

extern u16 D_80162B46;
extern u16 D_80162B48;
extern s16 D_80162B4A[];
extern u8 D_80161B45[];
extern u8 D_801398B8;
extern u8 D_80162EE2[];
extern char *func_80083C90(char *dst, char *src);
extern void func_8005483C(void);
extern void func_8005457C(u8 *buf);

s32 func_80054B48(void)
{
    u8 buf[0x100];

    if (D_80162B46 == D_80162B48) {
        return 1;
    }
    func_80083C90((char *)buf, (char *)(D_80161B45 + D_80162B4A[D_80162B46]));
    D_801398B8 = 1;
    if (D_80162EE2[D_80162B46]) {
        func_8005483C();
    }
    func_8005457C(buf);
    D_801398B8 = 0;
    if (++D_80162B46 == 0x1CC) {
        D_80162B46 = 0;
    }
    return 0;
}
