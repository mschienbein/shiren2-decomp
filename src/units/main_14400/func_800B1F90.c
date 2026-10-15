#include "common.h"
#include "map_rect.h"
typedef struct { MapRect geometry; unsigned char side_counts[4]; } Record;
extern Record D_801431F0[];
extern s32 func_800B200C(MapPos *pos);
void *func_800B1F90(void *pos) {
    s32 index = func_800B200C(pos);
    return index >= 0 ? &D_801431F0[index] : 0;
}
