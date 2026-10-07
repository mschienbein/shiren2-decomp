#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[6]; u8 limit; } Info;
extern s32 func_800A44F4(void *, void *);
extern void *func_800A492C(void *, s32, s32, s32);
extern s32 func_800A65B8(void *, void *);
extern s32 func_800E0F40(void *);
extern Info *func_80044E24(void *, u8);

void *func_8010AB7C(void *ctx, void *item) {
    s32 count;
    void *found;

    if (item == 0) {
        return 0;
    }
    found = item;
    if (func_800A44F4(ctx, found) == 1) {
        found = func_800A492C(ctx, 2, 1, 1);
    }
    if (found != 0) {
        count = func_800A65B8(ctx, found);
        if (func_80044E24(ctx, func_800E0F40(ctx))->limit < count) {
            found = 0;
        }
    }
    return found;
}
