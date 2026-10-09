#include "common.h"
typedef struct { unsigned char kind; s32 value; } Event;
extern s32 func_8012AC2C(void *item);
s32 func_8012A84C(s32 value) {
    Event event;
    event.value = value;
    event.kind = 0;
    return func_8012AC2C(&event);
}
