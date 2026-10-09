#include "common.h"
typedef struct { unsigned char pad00[0x1E]; unsigned char flags1E; } Item;
s32 func_800A7DAC(Item *item) { return (item->flags1E & 0x7C) != 0; }
