#include "common.h"
typedef struct { signed char value; } Direction;
/* Kind 19 senders (func_800FC7EC, func_8011DD9C) store the source unit pointer at +4. */
typedef struct { s32 kind; void *source_04; s32 reserved_08[2], value_10; Direction *direction_14; } Event;
extern void func_800EA784(void *, Event *), func_800EA7D4(void *), func_800EA80C(void *, Event *);
extern void func_800EA8FC(void *, Event *), func_800EA998(void *, Event *), func_800EAC8C(void *, Event *);
extern s32 func_800EAAC0(void *, void *, s32, Direction), func_800EAE8C(void *, Event *);
extern s32 func_800EAF38(void *, s32), func_800EAF7C(void *, s32), func_800EAFB8(void *, s32);
extern s32 func_800E4B60(void *, Event *);
s32 func_800EA67C(void *self, Event *event) {
    switch (event->kind) {
    case 2: func_800EA784(self, event); return 1;
    case 4: func_800EA7D4(self); return 1;
    case 5: func_800EA80C(self, event); return 1;
    case 8: func_800EA8FC(self, event); return 1;
    case 9: func_800EA998(self, event); return 1;
    case 19: return func_800EAAC0(self, event->source_04, event->value_10, *event->direction_14);
    case 20: func_800EAC8C(self, event); return 1;
    case 21: return func_800EAE8C(self, event);
    case 22: func_800EAF38(self, 1); return 1;
    case 23: return func_800EAF7C(self, 1);
    case 24: func_800EAFB8(self, 1); return 1;
    default: return func_800E4B60(self, event);
    }
}
