#include "common.h"
#include "map_rect.h"
typedef struct { unsigned char prefix[0xC]; MapRect bounds; } ActorBounds800A63FC;
extern s32 func_800A24DC(MapPos *, MapRect *);
s32 func_800A63FC(ActorBounds800A63FC *a, MapPos *b){ return func_800A24DC(b, &a->bounds); }
