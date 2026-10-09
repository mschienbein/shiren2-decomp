#include "common.h"
typedef struct { unsigned char value; } Dir;
typedef struct { s32 x; s32 y; } Point8010E7B0;
typedef struct {
    s32 kind; void *sender; void *target;
    Dir direction; unsigned char pad_0D[3];
    Point8010E7B0 position; unsigned char pad_18[8];
} Message8010E7B0;
typedef struct {
    unsigned char pad_00[0x38]; short adjustment_38; unsigned short pad_3A;
    s32 (*action_3C)(void *, Message8010E7B0 *);
} VTable8010E7B0;
typedef struct {
    unsigned char kind; unsigned char pad_01[7]; VTable8010E7B0 *vtable_08;
} Item8010E7B0;
extern void *func_800B4D80(Point8010E7B0 *position);
static __inline__ unsigned char direction_value(Dir *direction) {
    return direction->value;
}
/* The unused effect receiver remains the first argument in the original call contract. */
void func_8010E7B0(void *unused, Point8010E7B0 *position, Dir direction) {
    Item8010E7B0 *item = func_800B4D80(position);
    if (item != 0 && item->kind == 0x10) {
        Message8010E7B0 message;
        Message8010E7B0 *command;
        message.kind = 0x15;
        message.position = *position;
        command = &message;
        command->direction.value = direction_value(&direction);
        item->vtable_08->action_3C((unsigned char *)item + item->vtable_08->adjustment_38, command);
    }
}
