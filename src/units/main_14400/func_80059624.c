#include "common.h"

typedef struct {
    s32 a;
    s32 b;
    s32 c;
} Vec3i;

extern Vec3i D_80165324;

void func_80059624(Vec3i *dst) {
    *dst = D_80165324;
}
