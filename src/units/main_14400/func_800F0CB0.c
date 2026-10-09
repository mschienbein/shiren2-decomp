#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct Object Object;
typedef struct { u8 pad_00[0x40]; s16 delta; s16 pad_42; s32 (*call)(void *, Object *, u8 *); } VTable;
struct Object { u8 pad_00[0x24]; VTable *field_24; u8 pad_28[0x30]; Object *field_58; u8 pad_5C[0x3E]; u16 field_9A; u8 pad_9C[3]; u8 field_9F; };
typedef Object Obj;
typedef Object Unit;
extern void func_800E20F0(Obj *obj);
extern Unit *func_800C5F60(void);
extern u8 func_800A6420(Object *obj, Object *target);

void func_800F0CB0(Object *object, Object *target) {
    u8 state;
    func_800E20F0(object);
    if (target) {
        s32 result;
        s32 visible;
        Object *player;
        state = 0;
        result = object->field_24->call((u8 *)object + object->field_24->delta, target, &state);
        if (result == 0) {
            object->field_9A |= 0x20;
        }
        visible = 0;
        player = func_800C5F60();
        if (target == player || func_800A6420(object, object->field_58)) {
            if (func_800A6420(object, player)) {
                visible = 1;
            }
        }
        if (visible && result != 1) {
            object->field_58 = target;
            object->field_9F = state;
        }
    }
}
