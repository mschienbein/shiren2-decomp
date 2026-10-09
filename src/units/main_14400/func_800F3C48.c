#include "common.h"

typedef struct {
    unsigned short fields_00[62];
    unsigned short flags_7C;
} FlagView;

s32 func_800F3C48(FlagView *object) {
    return (object->flags_7C >> 7) & 1;
}
