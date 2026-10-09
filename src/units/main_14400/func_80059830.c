#include "common.h"

typedef struct { float x, y, z; } CameraVector;
extern CameraVector D_80165330;
extern s32 func_80059868(float angle);

s32 func_80059830(s32 direction)
{
    s32 offset = func_80059868(D_80165330.y);
    offset -= 14;
    direction -= offset;
    return direction & 7;
}
