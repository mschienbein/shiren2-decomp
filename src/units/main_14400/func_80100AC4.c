#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 field_00[0x8A]; u8 field_8A; } Actor;
typedef struct { u8 field_00[0x10]; Actor *field_10; } Object;
/* Whole 12-byte selection record; the mode is the byte at +8. */
typedef struct {
    unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode;
    signed char coordinates[2], status;
} SelectionRecord;
extern SelectionRecord D_80142F18;
extern s32 func_80049CB4(s32 id, ...);
extern void func_800A06F4(void *ctx, u16 id, Actor *obj, s32 mode, s32 flag);
/* D_8015B368+0x14 slot override. The dispatcher func_800C4E50 forwards a1..a3 to this slot
 * (jalr at 0x800C4E6C); direction is the forwarded byte pointer, unused here. */
void func_80100AC4(Object *object, Actor *target, u8 *direction, void *context) {
    if ((D_80142F18.mode ^ 0x4F) == 0) {
        /* Event 0x109 selects handler 0x13, which reads one context pointer. */
        func_80049CB4(0x109, context);
    } else {
        Actor *actor = object->field_10;
        func_800A06F4(context, actor->field_8A, target, 2, target != actor);
    }
}
