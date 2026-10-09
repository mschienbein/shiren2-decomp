#include "common.h"
typedef short s16;
typedef struct { s32 unk0, unk4; s16 unk8, unkA; void (*unkC)(void *, s32); } Dispatch;
typedef struct { s32 unk0, unk4; Dispatch *unk8; } State;
typedef struct { s32 value; } EventCode;
typedef struct {
    s32 kind;
    void *source;
    void *target;
    unsigned char pad_0C[0xC];
    s32 enable, check;
} Message;
extern void func_8010D910(State *, Message *);
extern void func_8010DA74(State *, Message *);
extern void func_8010DC80(State *, Message *);
extern void func_800D3650(State *);
extern s32 func_8010C47C(State *, Message *);
static inline s32 handle_event(EventCode code, State *arg0, Message *arg1) {
    switch (code.value) {
    case 16: func_8010D910(arg0, arg1); return 1;
    case 15: func_8010DA74(arg0, arg1); return 1;
    case 18:
        func_8010DC80(arg0, arg1);
        func_800D3650(arg0);
        if (arg0) {
            Dispatch *dispatch = arg0->unk8;
            dispatch->unkC((char *)arg0 + dispatch->unk8, 3);
        }
        return 1;
    case 19: func_8010DC80(arg0, arg1); return 1;
    default: return func_8010C47C(arg0, arg1);
    }
}
s32 func_8010D83C(State *arg0, Message *arg1) {
    EventCode code;
    code.value = arg1->kind;
    return handle_event(code, arg0, arg1);
}
