#include "common.h"
extern s32 D_80158418[];
void *func_800DA904(void *obj, s32 kind, unsigned char *params);
void *func_800DB0BC(void *self, unsigned char *arg) {
    func_800DA904(self, 14, arg);
    ((void **)self)[1] = D_80158418;
    return self;
}
