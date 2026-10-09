#include "common.h"

typedef struct {
    unsigned char fields_00[0x1E];
    unsigned char flags_1E;
    unsigned char fields_1F[0x7B];
    unsigned short flags_9A;
} Object;

extern Object *func_800C5F60(void);

/* The virtual predicate supplies arg0 even though this override ignores it. */
s32 func_800F8754(void *arg0, Object *object, unsigned char *output) {
    s32 selected;
    unsigned char flags;
    *output = 0;
    if (object == 0) {
        return 0;
    }
    selected = 0;
    if (((object->flags_1E >> 2) & 1) || object == func_800C5F60()) {
        selected = 1;
    }
    if (selected != 0) {
        return 1;
    }
    flags = object->flags_1E;
    if ((flags >> 3) & 1) {
        return 1;
    }
    if ((flags >> 4) & 1) {
        return ((object->flags_9A & 0x40) == 0) * 2;
    } else {
        return 0;
    }
}
