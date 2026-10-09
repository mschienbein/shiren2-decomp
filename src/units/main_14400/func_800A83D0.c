#include "common.h"

typedef unsigned char u8;
typedef struct Object Object;
extern s32 func_800A3B10(u8);
extern void *func_800A86EC(u8 kind);
extern s32 func_800A3A7C(u32 arg0);
extern Object *func_800A85A0(unsigned char, unsigned char);
extern s32 func_800A3AF0(u8);
extern void *func_800A8694(u8 kind, u8 variant, void *memory);
extern s32 func_800A3B00(u8);
extern void *func_800A8640(s32 command, s32 flags);
extern s32 func_800A3AE0(u8);
extern void *func_800A85F4(u8 kind);

void *func_800A83D0(s32 arg, unsigned char flags) {
    u8 kind = arg;
    if (func_800A3B10(kind)) {
        return func_800A86EC(kind);
    }
    if (func_800A3A7C(kind)) {
        return func_800A85A0(kind, flags);
    }
    if (func_800A3AF0(kind)) {
        return func_800A8694(kind, flags, 0);
    }
    if (func_800A3B00(kind)) {
        return func_800A8640(kind, flags);
    }
    if (func_800A3AE0(kind)) {
        return func_800A85F4(kind);
    }
    return 0;
}
