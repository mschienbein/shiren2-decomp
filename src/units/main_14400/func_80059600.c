#include "common.h"

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec3i;

extern Vec3i D_80165324;

void func_80059600(Vec3i *src) {
    D_80165324 = *src;
}
