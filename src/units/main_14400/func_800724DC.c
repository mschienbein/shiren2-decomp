#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef union FrameData { u32 segmented; void *resident; } FrameData;
typedef struct SpriteFrame { FrameData data; u16 size_04; u8 width, height; u32 field_08; } SpriteFrame;
extern u8 *D_8013D510;
extern s32 func_8007248C(u32 arg0, void *arg1, s32 arg2);
extern void func_800D8E60(u8 *dst, u8 *buf);
extern void func_80034720(void *addr, s32 size);
s32 func_800724DC(SpriteFrame *frame, void *destination) {
    s32 size = (frame->width * frame->height) >> 1;
    u8 *tail;
    s32 i;
    if (!D_8013D510 || func_8007248C(frame->data.segmented, D_8013D510, frame->size_04)) return -1;
    tail = (u8 *)destination + size;
    func_800D8E60(D_8013D510, destination);
    for (i = 0; i < (frame->width >> 1); ) {
        ++i;
        *tail++ = 0;
    }
    func_80034720(destination, size + (frame->width >> 1));
    return 0;
}
