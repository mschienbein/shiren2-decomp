#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 active;
    u8 pad1[3];
    s32 value4;
    s32 size8;
    u8 *bufferC;
} Ctx8008DD1C;

extern void func_8008DEC0(Ctx8008DD1C *ctx);
extern u8 *func_80091450(u32 size);
extern void func_80091544(u8 *buffer);
extern void func_8006AAF0(u8 *buffer, u32 source, s32 size);
extern void *func_80032D94(void *dst, const void *src, u32 size);

s32 func_8008DD1C(Ctx8008DD1C *ctx, u32 source, s32 size) {
    s32 result = 0;
    u8 *temp = 0;
    s32 aligned;

    /* ODD_C: single-pass error block grouping the aligned and odd-address DMA allocation
     * paths before the shared stream cleanup; it also shapes the original saved-register
     * choice. */
    do {
        if (ctx->active) {
            func_8008DEC0(ctx);
        }
        aligned = (size + 1) & ~1;
        ctx->active = 1;
        ctx->value4 = 0;
        ctx->size8 = size;
        ctx->bufferC = func_80091450(aligned);
        if (ctx->bufferC == 0) {
            result = -1;
            break;
        }
        if (source & 1) {
            source -= 1;
            size = (size + 2) & ~1;
            temp = func_80091450(size);
            if (temp == 0) {
                result = -1;
                break;
            }
            func_8006AAF0(temp, source, size);
            func_80032D94(ctx->bufferC, temp + 1, aligned);
            func_80091544(temp);
            temp = 0;
        } else {
            func_8006AAF0(ctx->bufferC, source, aligned);
        }
    } while (0);
    if (result != 0) {
        if (temp != 0) {
            func_80091544(temp);
        }
        func_8008DEC0(ctx);
    }
    return result;
}
