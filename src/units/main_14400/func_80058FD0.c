#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { float x, y, z; } Vector80058FD0;
extern s32 D_80165448;
extern Vector80058FD0 D_80165300;
extern s32 D_8016539C;
extern Vector80058FD0 D_8016534C;
extern Vector80058FD0 D_80165358;
extern s32 D_80165394;
extern s32 D_80165344;
extern s32 D_80165348;
extern Vector80058FD0 D_80165388;
void func_800591A0(void);
void func_80062BF0(s32 *a, s32 *b, s32 *c, s32 *d);
void func_800598B4(u8 a, u8 b, u8 c, u8 d);
void func_800265E0(void *buf, s32 size);
s32 func_800418A4(void);
void func_8005C4AC(Vector80058FD0 *a, Vector80058FD0 *b);
void func_8005A234(void);

void func_80058FD0(void) {
    s32 a;
    s32 b;
    s32 c;
    s32 d;

    func_800591A0();
    D_80165448 = 1;
    func_80062BF0(&a, &b, &c, &d);
    func_800598B4(a, b, c, d);
    func_800265E0(&D_80165300, 0xC);
    D_8016539C = func_800418A4();
    func_8005C4AC(&D_8016534C, &D_80165358);
    D_80165394 = 0;
    func_8005A234();
    D_80165344 = 0x14;
    D_80165348 = 0x800;
    func_800265E0(&D_80165388, 0xC);
}
