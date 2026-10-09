#include "common.h"

typedef struct {
    void *unk00;
    void *unk04;
    s32 unk08;
    const void *unk0C;
    s32 unk10[3];
    s32 unk1C;
} Object;
extern const unsigned char D_80154088[52];
extern Object *func_800C561C(Object *);
extern void func_800C5DC4(Object *);

Object *func_800C5D70(Object *object) {
    func_800C561C(object);
    object->unk0C = D_80154088;
    object->unk00 = &object->unk10;
    object->unk04 = &object->unk1C;
    func_800C5DC4(object);
    return object;
}
