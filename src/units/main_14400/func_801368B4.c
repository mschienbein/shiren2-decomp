#include "common.h"
typedef struct Pos Pos;
typedef Pos Pos800BE0A0;
extern u32 func_800B1C6C(Pos *pos);
s32 func_801368B4(Pos800BE0A0 *pos, s32 mask) {
    return (unsigned short)(func_800B1C6C(pos) & mask) != 0;
}
