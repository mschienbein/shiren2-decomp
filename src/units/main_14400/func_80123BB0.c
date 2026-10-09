#include "common.h"
typedef struct { s32 kind; void *source; unsigned char payload08[8]; s32 value; } Message;
typedef struct {
    unsigned char pad00[0x58]; short adjustment; unsigned short pad5A;
    s32 (*message)(void *, Message *);
} Vtable;
typedef struct { unsigned char pad00[0x24]; Vtable *vtable; } Object;
typedef struct Rng Rng;
extern Rng D_80147620;
extern unsigned char D_80156A59, D_80156A5B;
extern s32 func_800C5844(void *rng, unsigned char low, unsigned char high);
extern unsigned short func_80115944(void *owner, unsigned short value);
static inline Message *makeMessage(Message *message, void *source, unsigned short value) {
    message->kind = 11;
    message->source = source;
    message->value = value;
    return message;
}
/* Slot 0x44 receives seven pointers; origin, point, direction and item are unused. */
s32 func_80123BB0(void *owner, void *source, void *origin, void *point, void *direction, Object *target, void *item) {
    Message message;
    if (target != 0) {
        unsigned short value = func_80115944(owner, (unsigned char)func_800C5844(&D_80147620, D_80156A59, D_80156A5B));
        Message *command = makeMessage(&message, source, value);
        target->vtable->message((unsigned char *)target + target->vtable->adjustment, command);
    }
    return 1;
}
