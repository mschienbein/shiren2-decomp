#include "common.h"
typedef unsigned char u8;
typedef struct { u32 words[4]; } Signature;
extern s32 D_80138B00;
extern u32 D_8014A7EC[2];
extern Signature D_8014A7F4;
extern u8 *func_8006A810(u8 *dst, s32 value, s32 count);
/* PI device-bus addresses, not CPU-address pointer carriers. */
extern void func_80043C2C(u32 deviceAddress, void *destination, u32 length);
extern void func_80043D74(u32 deviceAddress, void *source, u32 length);
extern void func_80043BB8(unsigned long deviceAddress, unsigned long value);
extern s32 func_80083D8C(void *left, void *right, u32 count);

static inline s32 retry_allowed(s32 retries) {
    return retries < 2;
}

void func_80043768(void) {
    Signature signature;
    s32 retries;
    s32 i;
    D_80138B00 = 0;
    func_8006A810((u8 *)&signature, 0, 16);
    func_80043C2C(0x08007FF0, &signature, 16);
    if (func_80083D8C(&signature, &D_8014A7F4, 16) != 0) {
        Signature *expected;
        retries = 0;
        expected = &D_8014A7F4;
        for (; ; retries++) {
            if (!retry_allowed(retries)) break;
            func_80043BB8(0x08005000, 0);
            func_80043BB8(0x08005004, 0);
            func_80043BB8(0x08007E00, 0);
            func_80043BB8(0x08007E04, 0);
            for (i = 0; i < 2; i++) {
                u32 deviceAddress = D_8014A7EC[i];
                func_80043BB8(deviceAddress, 0);
                func_80043BB8(deviceAddress + 4, 0);
            }
            signature = *expected;
            func_80043D74(0x08007FF0, &signature, 16);
            func_8006A810((u8 *)&signature, 0, 16);
            func_80043C2C(0x08007FF0, &signature, 16);
            if (func_80083D8C(&signature, expected, 16) == 0) return;
        }
        D_80138B00 = 1;
    }
}
