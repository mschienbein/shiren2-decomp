#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x88];
    s32 field_88;
} Node;

typedef struct {
    u8 pad0[8];
    Node *node;
} Owner;

typedef struct {
    u8 pad0[0x1C];
    s32 base;
} Context;

typedef struct Event {
    struct Event *next;
    s32 field_4;
    u16 type;
    u16 padA;
    s32 value;
} Event;

extern Context *D_80148D84;

Event *func_80130780(void);
s32 func_8012E5F8(Node *node, s32 kind, Event *event);

void func_801300C0(Owner *owner, u8 value) {
    Event *event;

    if (owner->node == 0) {
        return;
    }
    event = func_80130780();
    if (event == 0) {
        return;
    }
    event->field_4 = D_80148D84->base + owner->node->field_88;
    event->type = 12;
    event->value = value;
    event->next = 0;
    func_8012E5F8(owner->node, 3, event);
}
