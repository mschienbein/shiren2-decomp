#include "common.h"
extern s32 D_801A70E0;
extern s32 (*D_8013D414[])(unsigned short *flags, void *other);
extern s32 func_8007E04C(void);
extern void func_8007D8DC(s32 value);
extern void func_8007D84C(s32 value);
extern void func_8007406C(s32 value);
extern void func_8006178C(s32 value);
extern void func_8005ADAC(s32 first, s32 second, s32 third);
s32 func_8006DBAC(unsigned short *flags, void *other) {
    if (*flags & 0x1000) {
        if (func_8007E04C()) {
            func_8007D8DC(0);
            func_8007D84C(1);
            func_8007406C(0);
            func_8006178C(0);
        }
        func_8005ADAC(0, 0, 1);
        return -1;
    }
    func_8007D84C(0);
    func_8007406C(1);
    func_8006178C(1);
    return D_8013D414[D_801A70E0](flags, other);
}
