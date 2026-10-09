#include "common.h"
typedef unsigned char u8;
typedef struct Target Target;
/* 0x20-byte item message: kind 12 carries the current actor at +4 and the pot's first
 * contained item at +8. */
typedef struct { s32 field0; void *field4; void *field8; unsigned char padC[0x14]; } Request;
/* Contents vtable (D_80154550 for the PotContents member at item+0xC): entry 7
 * (+0x38/+0x3C) is the indexed getter func_800CE7A0(list, unsigned index) -> item. */
typedef struct { u8 pad0[0x38]; short adjust; unsigned short reserved; void *(*get)(void *self, u32 index); } ContentsVTable;
typedef struct { s32 field0; ContentsVTable *field4; } Part;
/* Item vtable entry 7 (+0x38/+0x3C): message handler returning nonzero on success. */
typedef struct { u8 pad0[0x38]; short adjust; unsigned short reserved; s32 (*handle)(void *self, Request *request); } ItemVTable;
struct Target { u8 pad0[8]; ItemVTable *field8; Part fieldC; };
typedef struct { unsigned char pad[0xC]; Target *fieldC; } Object;
extern void *D_801476B8;
extern void func_800DAD20(Object *, Target *);
extern s32 func_80049CB4(s32, ...);
extern void *func_8011422C(Target *);
extern void func_800CD304(void *, unsigned int);
extern void func_800DAD80(Object *);
static inline Request *initialize(Request *request, void *value) {
    request->field0 = 12;
    request->field8 = value;
    request->field4 = D_801476B8;
    return request;
}
s32 func_800DD71C(Object *obj) {
    Target *target = obj->fieldC;
    Part *part;
    Request request;
    void *value;
    func_800DAD20(obj, target);
    func_80049CB4(0x28, D_801476B8);
    part = &target->fieldC;
    value = part->field4->get((unsigned char *)part + part->field4->adjust, 0);
    if (target->field8->handle((unsigned char *)target + target->field8->adjust, initialize(&request, value))) {
        func_800CD304(func_8011422C(target), 0);
    }
    func_800DAD80(obj);
    return 0;
}
