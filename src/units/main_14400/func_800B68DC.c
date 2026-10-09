#include "common.h"
/* Room rectangle followed by four per-side exit counts at +0x10. */
typedef struct { unsigned char rectangle[0x10]; unsigned char exits[4]; } Room;
void func_800B68DC(Room *room, s32 index) { ++room->exits[index]; }
