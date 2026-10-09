#include "common.h"
typedef struct { unsigned char field_00[0x18]; short field_18; void (*field_1c)(void *, s32, void *); } Methods;
typedef struct { unsigned char field_00[0x18]; Methods *field_18; } Object;
extern unsigned short func_800CA584(Object *, const unsigned char *);
void func_800CA4A4(Object *self, const unsigned char *text) {
    unsigned short value = func_800CA584(self, text);
    Methods *methods = self->field_18;
    methods->field_1c((unsigned char *)self + methods->field_18, 2, &value);
}
