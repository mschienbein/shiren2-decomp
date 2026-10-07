#include "common.h"

/* 46-byte records. */
typedef struct {
    unsigned char data[46];
} Rec80079448;

extern Rec80079448 D_801A79E8[];

Rec80079448 *func_80079448(s32 index)
{
    return &D_801A79E8[index];
}
