#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

extern u8 D_801D4D08[];
void func_80130B80(void);
void *func_800262C0(const void *source, void *destination, s32 size);
void func_80130BA8(void);
void func_80130A00(void *destination) {
    func_80130B80();
    func_800262C0(D_801D4D08, destination, 0x18);
    func_80130BA8();
}
