#include "common.h"

typedef unsigned char u8;
typedef struct {
    u8 field_00[0x28];
    short field_28;
    short field_2A;
    void (*field_2C)(void *, s32, void *);
} Methods;
typedef struct {
    u8 field_00[0x18];
    Methods *field_18;
} Object;
/* "Ring": NUL-terminated type-name tag in rodata, read byte-wise by func_800CA584. */
extern const char D_8015D87C[];
extern void func_800AF174(void *, Object *);
extern void func_800CA4E8(Object *, const char *name);

void func_80113B94(u8 *source, Object *object) {
    func_800AF174(source, object);
    func_800CA4E8(object, D_8015D87C);
    object->field_18->field_2C((u8 *)object + object->field_18->field_28, 2, source + 0xC);
}
