#include "common.h"
typedef struct { unsigned char pad00[0xA]; unsigned char type; } Item;
extern s32 func_800A3A30(unsigned char type);
s32 func_800A7D80(Item *item) { return func_800A3A30(item->type); }
