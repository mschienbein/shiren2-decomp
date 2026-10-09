#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad00[0x1C4]; u8 field1C4[0x10]; } Object;
extern void func_80048728(void *);

void func_8009F178(Object *self) {
    func_80048728(self->field1C4);
}
