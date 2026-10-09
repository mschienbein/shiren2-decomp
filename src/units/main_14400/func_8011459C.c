#include "common.h"
typedef unsigned char u8;
typedef signed short s16;
typedef struct { s32 x, y; } Position;
/* Message +0x4 is the actor pointer slot (null for this broadcast). */
typedef struct { s32 kind; void *actor4; void *sender8; s32 fieldC; Position pos10; s32 value18; } Message;
typedef struct { u8 pad0[0x38]; s16 adjust38; s16 pad3A; s32 (*dispatch3C)(void *, void *); } Methods;
typedef struct { u8 pad0[8]; const Methods *methods8; } Ent;
typedef struct { s32 index0; void *container4; s32 active8; void *currentC; } Iter;
typedef Iter S;
typedef struct Object Object;
extern void *func_8011422C(u8 *);
extern S *func_800CEB20(S *s, void *a);
extern s32 func_800CEBA0(Iter *);
extern Ent *func_800CEC68(Iter *);
extern s32 func_800CD2BC(Object *object, void *element);
extern s32 func_80049CB4(s32 id, ...);
s32 func_8011459C(void *obj, Position *position) {
    Iter iterator;
    Message message;
    s32 removed;
    Message *msg;
    s32 invalid;
    func_800CEB20(&iterator, func_8011422C(obj));
    removed = 0;
    msg = &message;
    invalid = -1;
    while (func_800CEBA0(&iterator)) {
        Ent *item;
        message.kind = 0x1B;
        message.actor4 = 0;
        message.sender8 = obj;
        message.pos10 = *position;
        msg->value18 = invalid;
        func_80049CB4(6);
        item = func_800CEC68(&iterator);
        if (item->methods8->dispatch3C((u8 *)item + item->methods8->adjust38, msg)) {
            func_800CD2BC(func_8011422C(obj), item);
            removed = 1;
        }
        func_80049CB4(7);
    }
    func_80049CB4(0x132);
    return removed;
}
