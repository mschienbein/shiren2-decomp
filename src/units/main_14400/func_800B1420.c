#include "common.h"
typedef struct { s32 x; s32 y; } Pos800B1420;
typedef struct { Pos800B1420 cur; Pos800B1420 start; Pos800B1420 end; } Iter800B1420;
extern unsigned short D_80143450[][76];
extern unsigned char D_80146468[][76], D_80145460[][76];
extern void *func_800A3610(void *out, void *it);
void func_800B1420(Iter800B1420 *it) { Pos800B1420 position; for (;;) { s32 more = it->cur.x <= it->end.x; if (!more) break; func_800A3610(&position, it); D_80143450[position.x][position.y] = 0xC000; D_80146468[position.x][position.y] = 0xFF; D_80145460[position.x][position.y] = 0xFF; } }
