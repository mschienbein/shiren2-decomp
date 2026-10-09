#include "common.h"

typedef struct { unsigned char pad00[8]; unsigned char flags08; } Obj80094DAC;
typedef struct { unsigned char field00; unsigned char kind01; unsigned char field02; } Record;
extern Record D_80148780[3]; /* Initialization and serialization cover three records. */
extern const unsigned char D_8015488C[8];
extern s32 func_800D5374(s32 id);
extern Obj80094DAC *func_800C9E10(void);

s32 func_800D5404(s32 id)
{
    s32 enabled = 0;
    if (func_800D5374(id)) {
        enabled = (func_800C9E10()->flags08 & D_8015488C[2]) == 0;
    }
    if (enabled && D_80148780[id].kind01 == 4) return 1;
    return 0;
}
