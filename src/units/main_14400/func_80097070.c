#include "common.h"

typedef struct {
    unsigned char pad0[0x74];
    void *unk74;
} Obj;

extern char *func_800A3D9C(void *unit);
extern char *func_80083C90(char *dst, char *src);

void func_80097070(Obj *obj, s32 clear, char *dst) {
    if (clear == 0) {
        func_80083C90(dst, func_800A3D9C(obj->unk74));
    } else {
        *dst = 0;
    }
}
