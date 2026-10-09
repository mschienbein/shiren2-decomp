#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef struct { u16 kind; } Event;
typedef struct { u8 pad_00[6]; s8 field_06; } State;
typedef struct { u8 pad_00[8]; s16 delta_08; s16 pad_0A; Event *(*poll)(void *); } PollTable;
typedef struct { PollTable *table; } Poller;
typedef struct Obj80045250 Obj80045250;
extern s32 D_80147670;
extern Event *func_80093A60(State *);
extern void func_800C96FC(void);
extern Obj80045250 *func_800C9E10(void);
extern void func_80045250(Obj80045250 *, s32);
extern void func_800452C0(Obj80045250 *);
extern void *func_800D8FB0(u32);
extern Event *func_800DF770(void *, s32);
extern void func_800498E4(s32, ...);
extern void func_80049BF0(s32);
extern s32 func_80049CB4(s32, ...);
extern s32 func_80093CDC(State *);
extern Event *func_800D9E90(void *);
extern Event *func_800D94A0(void *);
extern Event *func_80092640(State *);

Event *func_80093B58(State *state, Poller *poller) {
    Event *event = func_80093A60(state);
    if (event) {
        if (event->kind == 0x30) {
            func_800C96FC();
            if (state->field_06 == 2) {
                func_80045250(func_800C9E10(), 1);
            }
        }
        return event;
    }
    if (state->field_06 == 1) {
        void *storage = func_800D8FB0(12);
        return func_800DF770(storage, (D_80147670 ^ 3) == 0 ? 4 : 3);
    }
    if (state->field_06 == 2) {
        func_800498E4(0x221);
        func_80049BF0(0);
        func_80049CB4(2);
        state->field_06 = 0;
    }
    func_80045250(func_800C9E10(), 1);
    if (func_80093CDC(state)) {
        return func_800D9E90(func_800D8FB0(8));
    }
    if (poller) {
        event = poller->table->poll((u8 *)poller + poller->table->delta_08);
        if (!event) {
            event = func_800D94A0(func_800D8FB0(8));
        }
    } else {
        event = func_80092640(state);
    }
    func_800452C0(func_800C9E10());
    return event;
}
