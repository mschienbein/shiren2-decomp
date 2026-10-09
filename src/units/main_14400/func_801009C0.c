#include "common.h"

/* Original vtable (D_8015B368: entry functions at +0xC func_80100A10, +0x14 func_80100AC4);
 * its full layout is not modeled here, so it stays an opaque object. */
typedef struct VTable801009C0 VTable801009C0;

typedef struct {
    short id_0;
    char pad2[2];
    s32 field_4;
    s32 field_8;
    const VTable801009C0 *vtbl_C;
    void *owner_10;
} Obj_801009C0;

extern const VTable801009C0 D_8015B368;
extern Obj_801009C0 *func_800C4EC0(Obj_801009C0 *obj, s32 id, s32 a, s32 b);

Obj_801009C0 *func_801009C0(Obj_801009C0 *obj, void *owner) {
    func_800C4EC0(obj, 0x22F, 0x112, 0x111);
    obj->vtbl_C = &D_8015B368;
    obj->owner_10 = owner;
    return obj;
}
