#include "common.h"

typedef unsigned char u8;
typedef float f32;

typedef struct {
    u8 pad0[0x18];
    f32 matrix_18[9];
    u8 pad3C[0x4];
    s32 index_40;
} Src8008EDA8;

typedef struct { u8 pad0[0x8]; void **table_8; } Table8008EDA8;
typedef struct { u8 pad0[0x74]; Table8008EDA8 *table_74; } Owner8008EDA8;

typedef struct {
    u8 pad0[0x17C];
    f32 matrix_17C[9];
    u8 pad1A0[0x14];
    void *handle_1B4;
} Dst8008EDA8;

s32 func_8008D4B8(void *queue, void *item, s32 key);

s32 func_8008EDA8(Src8008EDA8 *src, Owner8008EDA8 *owner, Dst8008EDA8 *dst) {
    dst->matrix_17C[0] = src->matrix_18[0];
    dst->matrix_17C[1] = src->matrix_18[1];
    dst->matrix_17C[2] = src->matrix_18[2];
    dst->matrix_17C[3] = src->matrix_18[3];
    dst->matrix_17C[4] = src->matrix_18[4];
    dst->matrix_17C[5] = src->matrix_18[5];
    dst->matrix_17C[6] = src->matrix_18[6];
    dst->matrix_17C[7] = src->matrix_18[7];
    dst->matrix_17C[8] = src->matrix_18[8];
    return func_8008D4B8(dst->handle_1B4, owner->table_74->table_8[src->index_40], 0);
}
