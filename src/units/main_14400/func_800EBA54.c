#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x3]; u8 kind; } Result800EBA54;
typedef struct { u8 pad0[0x38]; s16 delta; s16 pad3A; Result800EBA54 *(*fn)(void *self, u32 index); } Vtbl800EBA54;
typedef struct { s32 unk0; Vtbl800EBA54 *vtable; } Member800EBA54;
typedef struct { u8 pad0[0x9]; u8 kind; u8 padA[0xE6]; Member800EBA54 member; } Obj800EBA54;
void func_800CFB90(Member800EBA54 *member, Obj800EBA54 *owner);
Member800EBA54 *func_800EBA54(Obj800EBA54 *obj) {
    Member800EBA54 *member = &obj->member;
    Vtbl800EBA54 *vt;
    Result800EBA54 *res;
    func_800CFB90(member, obj);
    vt = member->vtable;
    res = vt->fn((u8 *)member + vt->delta, 0);
    if (res != 0) {
        if ((obj->kind & 0xF) != res->kind) {
            return 0;
        }
        return member;
    }
    return 0;
}
