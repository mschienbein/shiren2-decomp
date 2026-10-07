#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x2C];
    s32 mode2C;
} Obj80045250;

extern s32 D_80138BC0;
extern void func_800452C0(Obj80045250 *obj);
extern void func_800CB050(Obj80045250 *obj);
extern u32 func_8006C550(void);

void func_80045250(Obj80045250 *obj, s32 mode) {
    if (mode != obj->mode2C) {
        obj->mode2C = mode;
        switch (mode) {
            case 0:
                func_800452C0(obj);
                func_800CB050(obj);
                break;
            case 1:
                D_80138BC0 = func_8006C550();
                break;
        }
    }
}
