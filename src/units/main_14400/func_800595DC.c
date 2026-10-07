#include "common.h"

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec3i;

extern Vec3i D_80165388;

void func_800595DC(Vec3i *src) {
    D_80165388 = *src;
}
