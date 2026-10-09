#include "common.h"
typedef unsigned char u8;
typedef struct { float translation[3], rotation[3], scale[3]; } Transform;
typedef struct { u8 header[0x14]; s32 field14; Transform transform; s32 field3C, field40, field44; } Record;
typedef struct { u8 pad0[2]; u8 count; u8 pad3[5]; Record **records; } Collection;
extern void *func_80091450(u32);
extern u8 *func_8006A810(u8 *, s32, s32);
extern void func_8008D3A0(void *, s32, void (*)(void), s32 (*)(void *, void *, void *), void (*)(void *));
extern void func_8008EDA0(void), func_8008EE28(void *);
extern s32 func_8008EDA8(void *, void *, void *);
extern s32 func_8008DF04(void *);
extern u32 func_8008E0C4(void *, void *, u32);
extern void func_80091544(void *);
s32 func_8008EE30(void *stream, s32 size, Collection *collection) {
    s32 result = 0;
    Record *record;

    /* ODD_C: single-pass error block grouping transform allocation and payload-size
     * validation before the shared cleanup; it also shapes the prologue scheduling. */
    do {
        record = func_80091450(0x48);
        if (!record) {
            result = -1;
            break;
        }
        func_8006A810((u8 *)record, 0, 0x48);
        func_8008D3A0(record, 0x54525346, func_8008EDA0, func_8008EDA8, func_8008EE28);
        record->field14 = func_8008DF04(stream);
        func_8008E0C4(stream, &record->transform, 0x24);
        /* Rotations are stored in radians; convert to degrees. */
        record->transform.rotation[0] *= 57.29577950560105;
        record->transform.rotation[1] *= 57.29577950560105;
        record->transform.rotation[2] *= 57.29577950560105;
        record->field3C = func_8008DF04(stream);
        record->field40 = func_8008DF04(stream);
        if (size != 0x30) {
            result = -1;
            break;
        }
        collection->records[collection->count++] = record;
    } while (0);
    if (result && record) {
        func_80091544(record);
    }
    return result;
}
