#include "common.h"

typedef unsigned char u8;

typedef struct VTable801110A8 VTable801110A8;

typedef struct {
    char pad0[0x24];
    VTable801110A8 *vtbl;
} Obj801110A8;

struct VTable801110A8 {
    char pad0[0x78];
    short adjust78;
    void (*func7C)(void *self, short step);
};

typedef struct { s32 x, y; } Pos801110A8;
typedef struct {
    char pad0[0x10];
    Pos801110A8 pos;
} Info801110A8;

s32 func_8010BEC4(void *obj, u8 id);
s32 func_800FCF3C(void *pos, s32 mode);

void func_801110A8(void *arg0, Obj801110A8 *obj, Info801110A8 *info) {
    if ((u8)func_8010BEC4(arg0, 10)) {
        obj->vtbl->func7C((char *)obj + obj->vtbl->adjust78, -1);
    }
    if ((u8)func_8010BEC4(arg0, 25)) {
        func_800FCF3C(&info->pos, 1);
    }
}
