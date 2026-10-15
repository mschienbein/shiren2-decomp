#include "common.h"

typedef struct OSPfsState OSPfsState;
typedef struct { s32 unknown00[2]; s32 field08; } Object;
/* Payload of command 0x206, read by func_801314E4 as {file, state}. */
typedef struct { Object *object; OSPfsState *state; } Arguments;
extern s32 func_80131F04(s32, Arguments *);
void func_80130FE0(Object *object, OSPfsState *state) {
    Arguments arguments;
    arguments.state = state;
    arguments.object = object;
    object->field08 = func_80131F04(0x206, &arguments);
}
