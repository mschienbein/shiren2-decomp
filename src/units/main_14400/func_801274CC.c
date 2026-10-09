#include "common.h"
typedef struct { s32 field_00, field_04; } Position;
typedef struct { unsigned char field_00[8]; short field_08; void (*field_0C)(void *, s32); } VTable;
typedef struct { Position field_00; VTable *field_08; s32 field_0C; unsigned char field_10[0xC]; unsigned short field_1C; } Object;
typedef struct { s32 field_00; Object *field_04; } Message;
extern s32 D_801487B0;
extern unsigned char D_80156AB7, D_80156AB9, D_80156ABB, D_801CA680[3], D_801CA67F[];
extern short D_801CA684[30];
extern char *func_800A3B20(void *obj), *func_800AE674(void *obj);
extern void func_800498E4(s32, ...);
extern s32 func_80049CB4(s32, ...);
extern Position func_800A6B70(Object *, unsigned char, s32);
extern Object *func_800B4928(Position *), *func_800A8CB0(s32);
extern s32 func_800A5388(Object *, s32);
extern unsigned char func_800A8C00(Object *);
extern void func_800A7B18(Object *, Object *, s32, s32), func_800D3650(Object *);
extern s32 func_80112B38(Object *, Message *);
static inline void copy_position(Position *to, Position *from) { to->field_00 = from->field_00; to->field_04 = from->field_04; }
static inline unsigned char *get_values(void) { return D_801CA680; }
static inline void init_values(void) { unsigned char *values = get_values(); D_801487B0 = 1; values[0] = D_80156AB7; values[1] = D_80156AB9; values[2] = D_80156ABB; }
static inline s32 clear_flag(s32 value, s32 mask) { return value & mask; }
s32 func_801274CC(Object *object, Message *message) {
    Position origin, target_position; Object *owner; s32 any, remaining;
    if (!D_801487B0) init_values();
    if (message->field_00 == 7) {
    { s32 i, offset; for (i = 29, offset = 58; i >= 0; i--, offset -= 2) *(short *)((char *)D_801CA684 + offset) = 0; }
    owner = message->field_04; any = 0; copy_position(&origin, &owner->field_00);
    { char *name = func_800A3B20(owner); func_800498E4(0x11C, name, func_800AE674(object)); }
    func_80049CB4(0x10A5, owner); func_80049CB4(0xCE, object, &origin); func_80049CB4(0xDD);
    remaining = D_801CA67F[object->field_0C];
    for (;;) {
        Object *target; s32 more = remaining-- > 0; if (!more) break;
        target_position = func_800A6B70(owner, 0x7C, 1);
        if (!(target_position.field_04 | target_position.field_00)) break;
        func_80049CB4(6); func_80049CB4(0xCF, object, &target_position); func_80049CB4(0x114, &target_position);
        target = func_800B4928(&target_position);
        if (!func_800A5388(target, 0x10)) { unsigned char index = func_800A8C00(target); if (!D_801CA684[index]) func_80049CB4(0x23, target, 1, 0); D_801CA684[index] += 7; }
        func_80049CB4(7); func_80049CB4(0x129, 5); any = 1;
    }
    if (!any) func_800498E4(0x223);
    else {
        func_80049CB4(0x129, 5);
        { s32 i; for (i = 30; ; ) { Object *target; s32 more = --i >= 0; if (!more) break; if (!D_801CA684[i]) continue; target = func_800A8CB0((unsigned char)i); target->field_1C |= 0x100; func_800A7B18(target, owner, D_801CA684[i], 6); target->field_1C = clear_flag(target->field_1C, ~0x100); } }
    }
    func_800D3650(object);
    if (object) object->field_08->field_0C((char *)object + object->field_08->field_08, 3);
    return 1;
    } else return func_80112B38(object, message);
}
