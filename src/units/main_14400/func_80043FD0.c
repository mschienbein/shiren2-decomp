#include "common.h"

typedef unsigned char u8;
typedef u32 size_t;
/* g++ vtable entry {delta, index, pfn}; entry 1 is the cache flush. */
typedef struct { short delta; short index; void (*fn)(void *); } VEntry;
/* Write-back device stream: position_00 is the stream offset, window_40 the
 * offset cached in cache_20 (-1 when empty) and dirty_44 marks unflushed data. */
typedef struct {
    s32 position_00; u8 pad_04[8]; s32 error_0C; u8 device_10; u8 pad_11[3];
    const char *message_14; VEntry *vtable_18; u32 address_1C; u8 cache_20[32];
    s32 window_40, dirty_44;
} Obj;
extern s32 D_80138B00;
extern s32 D_801541E4[];
extern const char D_8014A814[];
extern void func_80043ED8(Obj *obj);
extern void *func_80032D94(void *dst, const void *src, size_t size);

void func_80043FD0(Obj *obj, u32 size, const void *data)
{
    const u8 *src;
    u32 chunk;
    /* ODD_C: one status variable reads the stream error, then the global
     * write-protect flag, then holds the dirty flag stored per chunk; being
     * set three times it is not hoisted out of the loop by loop.c. */
    s32 status = obj->error_0C;
    s32 position;
    if (status != 0) {
        return;
    }
    src = data;
    status = D_80138B00;
    if (status != 0) {
        return;
    }
    position = obj->position_00;
    if (D_801541E4[obj->device_10] - position < (s32)size) {
        obj->message_14 = D_8014A814;
        return;
    }
    if (size == 0) {
        return;
    }
    do {
        if (obj->window_40 >= 0 &&
            (obj->position_00 < obj->window_40 || obj->position_00 >= obj->window_40 + 32)) {
            obj->vtable_18[1].fn((u8 *)obj + obj->vtable_18[1].delta);
            obj->window_40 = -1;
        }
        if (size >= 32 && (obj->window_40 < 0 || obj->position_00 == obj->window_40)) {
            obj->window_40 = obj->position_00;
            chunk = 32;
        } else {
            func_80043ED8(obj);
            chunk = 32 - (obj->position_00 - obj->window_40);
            if (size < chunk) {
                chunk = size;
            }
        }
        /* local-arithmetic-qualification: the destination is cache_20 at the
         * in-window offset (chunk never passes the 32-byte cache). Every pointer
         * spelling (&obj->cache_20[offset], obj->cache_20 + offset,
         * (u8 *)obj + offset + 0x20) adds the object base first and three of
         * them also move the +0x20 ahead of the call; only this integer sum
         * keeps the original "offset + base" operand order. */
        func_80032D94((u8 *)(obj->position_00 - obj->window_40 + (u32)obj) + 0x20, src, chunk);
        src += chunk;
        size -= chunk;
        status = 1;
        position = obj->position_00;
        obj->dirty_44 = status;
        position += chunk;
        obj->position_00 = position;
    } while (size != 0);
}
