#include "common.h"

typedef unsigned char u8;

typedef struct Member Member;

typedef struct {
    u8 pad0[4];
    u8 threshold;
} Requirement800CD508;

s32 func_800CD278(Member *member);

s32 func_800CD508(Member *member, Requirement800CD508 *requirement)
{
    return func_800CD278(member) >= requirement->threshold;
}
