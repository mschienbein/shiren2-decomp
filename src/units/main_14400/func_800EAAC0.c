#include "common.h"
typedef struct { s32 field_00, field_04; } Position;
typedef struct { signed char value; } Flag;
/* Kind 17 receivers dereference the source unit pointer at +4. */
typedef struct { s32 field_00; void *source_04; s32 field_08; unsigned char field_0C; unsigned char field_0D[3]; Position field_10; s32 field_18, field_1C; } Message;
typedef struct { unsigned char field_00[0x38]; short field_38; s32 (*field_3C)(void *, Message *); } TargetTable;
typedef struct { Position field_00; TargetTable *field_08; } Target;
typedef struct { unsigned char field_00[0x98]; short field_98; void *(*field_9C)(void *); } VTable;
typedef struct { Position field_00; unsigned char field_08[0x1C]; VTable *field_24; } Object;
extern char D_80147620[];
extern s32 func_800A692C(Object *, s32);
extern unsigned char func_800E8B10(Object *, Target **);
extern Target *func_800E8A68(Object *, unsigned char);
extern unsigned char func_800C57A0(void *);
extern s32 func_80049CB4(s32, ...);
extern void func_800498E4(s32, ...);
extern char *func_800A3B20(Object *), *func_800AE674(Target *);
extern s32 func_800CD2BC(void *, Target *);
static inline void copy_position(Position *to, const Position *from) { to->field_00 = from->field_00; to->field_04 = from->field_04; }
static inline void set_message_tail(Message *message) { message->field_18 = -1; message->field_1C = 0; }
static inline unsigned char flag_value(Flag *flag) { return flag->value; }
s32 func_800EAAC0(Object *object, void *source_04, s32 kind, Flag flag) {
    /* ODD_C: the target pair and notice position reuse one eight-byte frame
     * slot at sp+0x10 in the ROM; separate block locals miss 40 words. */
    union { Target *targets[2]; Position position; } scratch;
    Message message; Target *target;
    if (func_800A692C(object, 0x12)) return 0;
    target = 0;
    switch (kind) {
    case 3: target = func_800E8A68(object, 3); break;
    case 4: target = func_800E8A68(object, 4); break;
    case 6: { unsigned char count = func_800E8B10(object, scratch.targets); if (count == 2) target = scratch.targets[func_800C57A0(D_80147620) & 1]; else if (count == 1) target = scratch.targets[0]; break; }
    case 9: target = func_800E8A68(object, 9); break;
    }
    if (target) {
        if (source_04) {
            char *name;
            copy_position(&scratch.position, &object->field_00);
            func_80049CB4(0x124, &scratch.position);
            name = func_800A3B20(object);
            func_800498E4(0x56, name, func_800AE674(target));
        }
        func_800CD2BC(object->field_24->field_9C((char *)object + object->field_24->field_98), target);
        message.field_00 = 0x11;
        message.source_04 = source_04;
        message.field_10 = object->field_00;
        message.field_0C = flag_value(&flag);
        set_message_tail(&message);
        target->field_08->field_3C((char *)target + target->field_08->field_38, &message);
        return 1;
    }
    return 0;
}
