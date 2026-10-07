#include "common.h"

typedef struct {
    void *unk0;
    signed char unk4;
} Obj80094AFC;

extern void func_8009490C(Obj80094AFC *obj, void *msg);
extern void func_800CA2C0(void *arg0);

void func_80094AFC(Obj80094AFC *obj, void *msg) {
    if (msg != 0 && obj->unk4 == 0) {
        func_8009490C(obj, msg);
        func_800CA2C0(obj->unk0);
    }
}
