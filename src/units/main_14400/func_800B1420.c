#include "common.h"
#include "map_position_iterator.h"
extern unsigned short D_80143450[][76];
extern unsigned char D_80146468[][76], D_80145460[][76];
void func_800B1420(MapPositionIterator *it) { MapPos position; for (;;) { s32 more = it->cur.x <= it->end.x; if (!more) break; func_800A3610(&position, it); D_80143450[position.x][position.y] = 0xC000; D_80146468[position.x][position.y] = 0xFF; D_80145460[position.x][position.y] = 0xFF; } }
