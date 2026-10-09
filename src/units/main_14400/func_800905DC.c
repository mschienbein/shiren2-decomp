#include "common.h"

typedef unsigned char u8;

typedef struct Stream Stream;

/* Resource callbacks registered by func_8008D3A0. */
typedef void (*ResInitFn)(void);
/* The apply call supplies an owner pointer even when the target ignores it. */
typedef s32 (*ResLoadFn)(void *res, void *owner, void *ctx);
typedef void (*ResFreeFn)(void *res);

/* 0x24-byte 'TXIM' resource: registration header, then the loaded image block. */
typedef struct {
    s32 tag;
    s32 id;
    ResInitFn init;
    ResLoadFn load;
    ResFreeFn release;
    s32 field_14;
    s32 size;
    u8 *data;
    s32 field_20;
} Image;

#define TAG_TXIM 0x5458494D

void *func_80091450(u32 size);
u8 *func_8006A810(u8 *dst, s32 value, s32 count);
void func_8008D3A0(Image *rec, s32 tag, ResInitFn init, ResLoadFn load, ResFreeFn release);
s32 func_8008DF04(Stream *stream);
u32 func_8008E0C4(Stream *stream, void *dst, u32 len);
void func_80091544(void *item);
void func_80090580(void);
s32 func_80090588(void *res, void *unused_owner, void *ctx);
void func_800905A4(void *res);

/* Read one image resource of `size` bytes from the stream; NULL on failure. */
void *func_800905DC(Stream *stream, s32 size)
{
    s32 failed = 0;
    u8 *data = 0;
    Image *image;

    /* ODD_C: early-exit group; a failed allocation breaks to the shared cleanup that frees the
     * record and data. Also shapes scheduling: nested if/else and goto-done forms differ in 3 words. */
    do {
        image = func_80091450(sizeof(Image));
        if (image == 0) {
            failed = -1;
            break;
        }
        func_8006A810((u8 *)image, 0, sizeof(Image));
        func_8008D3A0(image, TAG_TXIM, func_80090580, func_80090588, func_800905A4);
        image->field_14 = func_8008DF04(stream);
        image->size = func_8008DF04(stream);
        data = func_80091450(image->size);
        if (data == 0) {
            failed = -1;
            break;
        }
        func_8008E0C4(stream, data, image->size);
        image->data = data;
        image->field_20 = 0;
        if (size != image->size + 8) {
            failed = -1;
        }
    } while (0);
    if (failed) {
        if (image != 0) {
            func_80091544(image);
            image = 0;
        }
        if (data != 0) {
            func_80091544(data);
        }
    }
    return image;
}
