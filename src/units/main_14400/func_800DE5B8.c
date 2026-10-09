#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct Obj800EB9FC Obj800EB9FC;
typedef union { s32 value; void *pointer; } EventArg;
/* The dispatch record occupies 0x20 bytes; kind 11 uses its first two arguments. */
typedef struct { s32 kind; EventArg args[7]; } Event;
typedef struct { u8 pad00[0x38]; s16 delta38, index3A; s32 (*dispatch3C)(void *, Event *); } VTable;
typedef struct { u8 pad00[8]; VTable *vtable08; } Target;
typedef struct { u8 pad00[0xB0]; u8 fieldB0[0x14]; Target *targetC4; } Object;
extern Obj800EB9FC *D_801476B8;
extern void *func_800EB9FC(Obj800EB9FC *obj);
extern char *func_80048480(u16 id);
extern void func_800498E4(s32 id, ...);

s32 func_800DE5B8(Object *self) {
    Event event;
    Event *message;
    Target *target;
    if (func_800EB9FC(D_801476B8) != 0) {
        func_800498E4(0xB6, func_80048480(0x46A));
        return 0;
    } else {
        message = &event;
        event.kind = 0xB;
        message->args[0].pointer = D_801476B8;
        message->args[1].pointer = self->fieldB0;
        target = self->targetC4;
        target->vtable08->dispatch3C((u8 *)target + target->vtable08->delta38, message);
        return 0;
    }
}
