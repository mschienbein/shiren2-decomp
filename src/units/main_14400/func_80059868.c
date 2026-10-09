#include "common.h"
extern const double D_8014C130, D_8014C138;
s32 func_80059868(float angle) {
    return (s32)(((double)angle + D_8014C130) / D_8014C138) & 7;
}
