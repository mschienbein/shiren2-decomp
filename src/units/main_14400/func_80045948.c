#include "common.h"

typedef signed char s8;
typedef short s16;

typedef struct {
    s8 x;
    s8 y;
} BytePair;

void func_80051E9C(s16 id, BytePair pair);

void func_80045948(s32 id, BytePair pair)
{
    func_80051E9C((s16)id, pair);
}
