#include "common.h"
typedef struct { unsigned char pad00[5]; signed char ownerId; } Item;
extern s32 func_800AE9AC(Item *item, s32 first, s32 second);
extern s32 func_8010BA90(Item *item, short value);
extern void func_800D3698(s32 owner, s32 delta);
s32 func_8010BAE8(Item *item, s32 value) {
    s32 before = func_800AE9AC(item, 0, 0);
    s32 result = func_8010BA90(item, (short)value);
    func_800D3698(item->ownerId, before - func_800AE9AC(item, 0, 0));
    return result;
}
