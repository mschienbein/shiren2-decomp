#include "common.h"

/* Stream object: +0x14 holds the failing tag/error text pointer (null when clean). */
typedef struct { unsigned char pad0[0x14]; const char *field14; } Object;
extern Object *D_80147F44;
extern unsigned char D_801476D0;
extern void *func_80044678(s32 kind, s32 a, s32 *status);
extern s32 func_800CBE44(void);
extern void func_800CC16C(void);

s32 func_800CBD2C(void)
{
    s32 status;
    Object *object = func_80044678(0, 0, &status);
    object->field14 = 0;
    D_80147F44 = object;
    D_801476D0 = 0;
    if (status == 3 || status == 1) {
        s32 reset = func_800CBE44() ^ 1;
        switch (reset) {
        case 0:
            break;
        default:
            func_800CC16C();
            D_801476D0 = 0;
            D_80147F44->field14 = 0;
            break;
        }
    }
    return status;
}
