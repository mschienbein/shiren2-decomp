#include "common.h"
typedef unsigned char u8;
typedef struct { s32 a, b; } Vec2;
typedef struct VTable VTable;
typedef struct {
    u8 pad0[0x1C]; const VTable *vtable1C;
    void *field20; void *field24; void *field28; Vec2 pos2C;
    u8 field34; u8 pad35[3]; s32 field38;
} Obj;
typedef Obj Effect;
typedef struct { Vec2 pos; u8 color; } Entity;
extern s32 func_80111A20(void *, void *);
extern Obj *func_80111E08(Obj *o, void *a1, void *a2, Vec2 *pos, u8 *color);
extern void func_800C2D0C(Effect *effect);
extern const VTable D_8015F2F8;
void func_8011F83C(void *owner, Entity *entity) {
    Obj effect;
    if (func_80111A20(owner, entity)) {
        Obj *effectPtr = &effect;
        func_80111E08(effectPtr, entity, owner, &entity->pos, &entity->color);
        effectPtr->vtable1C = &D_8015F2F8;
        effectPtr->field38 = 0;
        func_800C2D0C(effectPtr);
    }
}
