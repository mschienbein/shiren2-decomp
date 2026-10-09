#include "common.h"

typedef struct {
    unsigned char pad0[0x54];
    unsigned char field54[0x10];
} Object;

extern void func_80048728(void *);

void func_8009E1EC(Object *self)
{
    func_80048728(self->field54);
}
