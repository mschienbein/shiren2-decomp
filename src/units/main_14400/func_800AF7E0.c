#include "common.h"
#include "item_records.h"
s32 func_800AF7E0(ItemPool *pool) { s32 i; for(i=0;i<7;i++) { if(D_80153AE4[i]==pool) return i; } return -1; }
