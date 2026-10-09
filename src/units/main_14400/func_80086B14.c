#include "common.h"
typedef struct {
    unsigned char field_00[4]; short field_04;
    unsigned char field_06[8]; short field_0e;
    s32 field_10; s32 field_14; /* camera mode forwarded to func_8005A464 */
} Object;
extern void func_8005A464(s32, void *, s32, s32);
void func_80086B14(Object *self) {
    func_8005A464(self->field_14, 0, 0, 0);
    self->field_0e = 1;
    self->field_04 = 4;
}
