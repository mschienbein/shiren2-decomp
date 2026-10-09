#include "common.h"
typedef struct {
    unsigned char pad00[0x24]; const void *vtable;
    unsigned char pad28[0x72]; unsigned short flags9A;
} Object;
extern Object *func_800EFC70(Object *object, s32 kind, unsigned char index);
extern const unsigned char D_8015B440[];
Object *func_80100DE0(Object *object, unsigned char index) {
    func_800EFC70(object, 0x3A, index);
    object->vtable = D_8015B440;
    object->flags9A |= 1;
    return object;
}
