#include "common.h"
typedef struct { unsigned char value; } Dir;
typedef struct { s32 x; s32 y; } Point800D5968;
typedef struct {
    unsigned char pad_00[0x1E]; unsigned char flags_1E;
    unsigned char pad_1F[0x53]; unsigned char flags_72;
} Obj800D5968;
extern void *func_800A2594(Point800D5968 *out, void *source, Dir direction);
extern void *func_800B4928(Point800D5968 *position);
extern void func_800A2F80(Dir *direction, s32 amount);
static __inline__ s32 nonzero(s32 value) {
    return value != 0;
}
static __inline__ s32 is_blocked(Obj800D5968 *object) {
    return ((object->flags_1E >> 1) & 1) || (nonzero(object->flags_1E & 0x7C) & (object->flags_72 & 1));
}
s32 func_800D5968(void *source) {
    Point800D5968 position;
    Dir direction;
    s32 count = 8;
    direction.value = 2;
    for (;;) {
        Obj800D5968 *object;
        if (--count == -1) {
            break;
        }
        func_800A2594(&position, source, direction);
        object = func_800B4928(&position);
        if (object != 0) {
            s32 blocked = is_blocked(object);
            if (blocked) {
                return 0;
            }
        }
        func_800A2F80(&direction, 1);
    }
    return 1;
}
