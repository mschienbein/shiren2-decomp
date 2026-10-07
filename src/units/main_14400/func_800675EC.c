#include "common.h"
typedef struct { char pad0[0xC]; float x; float y; float z; } Pos;
extern Pos *D_8013B984;
void func_800675EC(float x, float y, float z) {
    Pos *p = D_8013B984;
    if (p != 0) {
        p->x = x;
        p->y = y;
        p->z = z;
    }
}
