#include "common.h"
typedef struct { char pad[0x54]; s32 unk54; } Obj8009F76C;
void func_8009DB9C(void *obj, void *value);
void func_8009F76C(Obj8009F76C *self, void *value) {
    if (self->unk54 == 0) {
        func_8009DB9C(self, value);
    }
}
