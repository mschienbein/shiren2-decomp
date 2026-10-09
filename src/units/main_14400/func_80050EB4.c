#include "common.h"
typedef struct { s32 x, y; } Point;
typedef struct { s32 x, y, z; } Position;
typedef struct { unsigned char unk00[0x12]; unsigned short unk12; } Object;
extern void *func_80085938(s32, s32, Position, s32, s32, s32, s32);
Object *func_80050EB4(s32 arg0, Point *point, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    Position position;
    Object *result;
    s32 vertical = point->y;
    s32 dx = arg4;
    s32 dy = arg5;
    s32 dz = arg6;
    vertical <<= 5;
    dx += 0x10;
    position.x = vertical + dx;
    dy += 0x10;
    position.y = (point->x << 5) + dy;
    position.z = dz;
    result = func_80085938(arg0, -1, position, arg3, arg2, 0, 0);
    result->unk12 |= 4;
    return result;
}
