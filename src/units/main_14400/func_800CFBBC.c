#include "common.h"

typedef struct { s32 x; s32 y; } Position;
/* func_800B4F74 reads both position words at object offsets 8 and 12. */
typedef struct { char pad[8]; Position position; } S;
extern s32 func_800B4F74(Position *);
s32 func_800CFBBC(S *s) {
    if (func_800B4F74(&s->position) != 0) return 1;
    return 10;
}
