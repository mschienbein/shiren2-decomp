#include "common.h"
typedef struct { void *room; s32 kind; s32 changed; s32 active; } RoomState;
extern RoomState D_80143434;
s32 func_800B60D0(void) { return D_80143434.room != 0; }
