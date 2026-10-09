#include "common.h"

typedef unsigned char u8;

/* Direction byte (0-7); bit 0 set means a diagonal facing. */
typedef union {
    u8 raw;
    struct {
        unsigned int upper : 7;
        unsigned int diagonal : 1;
    } bits;
} Direction;

typedef struct {
    char pad0[0x10];
    Direction dir;
} Obj;

extern void func_800A2F94(unsigned char *value, s32 delta);

/* Load the facing from the stream and turn diagonal facings one step. */
void func_801269C4(Obj *obj, u8 *src) {
    obj->dir.raw = *src;
    if (obj->dir.bits.diagonal) {
        func_800A2F94(&obj->dir.raw, 1);
    }
}
