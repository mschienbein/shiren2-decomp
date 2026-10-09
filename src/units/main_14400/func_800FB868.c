#include "common.h"
typedef struct Message {
    void *field_0;
    s32 field_4;
    s32 field_8;
    short field_C;
    unsigned short field_E;
    unsigned char field_10;
} Message;
/* Slot +0x6C targets the unsigned-word getter func_800E0E88. */
typedef struct { char fields_0[0x68]; short adjustment_68; u32 (*method_6C)(void *); } Interface;
typedef struct { char fields_0[0x24]; Interface *field_24; } Object;
extern unsigned char D_80147620[];
extern s32 func_800F069C(Object *);
extern s32 func_800F0EC4(Object *);
extern s32 func_800E0F40(Object *);
extern unsigned char func_800C57A0(void *);
extern s32 func_800E33D0(Object *, Message, unsigned char, unsigned char);
extern s32 func_800F3358(Object *);
static inline void initialize_action(Message *action, u32 value) {
    action->field_C = value;
    action->field_E = 0;
    action->field_8 = 0;
    action->field_10 = 10;
}
s32 func_800FB868(Object *object) {
    Message action, copy;
    s32 ready = 0;
    if (!func_800F069C(object)) ready = func_800F0EC4(object) == 0;
    if (ready) {
        initialize_action(&action, object->field_24->method_6C((char *)object + object->field_24->adjustment_68));
        switch (func_800E0F40(object) & 0xFF) {
        case 1: break;
        case 2:
            if (func_800C57A0(D_80147620) & 3) break;
        case 3: action.field_8 = 0xA1; break;
        default: action.field_8 = 0xA2; break;
        }
        copy = action;
        func_800E33D0(object, copy, 1, 1);
        return 1;
    }
    return func_800F3358(object);
}
