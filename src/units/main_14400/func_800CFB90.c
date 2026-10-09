#include "common.h"

typedef unsigned char u8;

typedef struct Pair800CFB90 {
    s32 first;
    s32 second;
} Pair800CFB90;

/* The owner's leading 8-byte pair is copied as a whole into member+8. */
typedef struct Obj800EBA54 {
    Pair800CFB90 pair_00;
} Obj800EBA54;

typedef struct Member800EBA54 {
    u8 pad_00[0x8];
    Pair800CFB90 pair_08;
} Member800EBA54;

void func_800CFB90(Member800EBA54 *member, Obj800EBA54 *owner) {
    member->pair_08 = owner->pair_00;
}
