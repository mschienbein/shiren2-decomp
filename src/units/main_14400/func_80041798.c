#include "common.h"
typedef struct { s32 x; s32 y; } Pos;
typedef struct { s32 x; s32 y; s32 z; s32 w; } Rect;
extern Rect *func_800B310C(Rect *out, Pos *pos);
void func_80041798(s32 y, s32 x, s32 *outY, s32 *outX, s32 *outW, s32 *outZ) {
    Rect rect;
    Pos pos;
    Rect *r;
    pos.y = y;
    pos.x = x;
    func_800B310C(&rect, &pos);
    r = &rect;
    *outY = r->y;
    *outX = rect.x;
    *outW = r->w;
    *outZ = r->z;
}
