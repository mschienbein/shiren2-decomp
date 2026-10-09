#include "common.h"

typedef unsigned char u8;
typedef struct Obj Obj;
/* The member-style receiver is supplied by callers but unused by this checksum step. */
void func_800CA0EC(Obj *obj, s32 *state, u8 value)
{
    u32 previous = *state;
    *state = ((previous << 8) | (((previous >> 10) ^ (previous >> 23)) & 0xFF)) ^ value;
}
