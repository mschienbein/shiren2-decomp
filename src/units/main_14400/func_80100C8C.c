#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct Object {
    char pad00[0x58];
    struct Object *target_58;
    char pad5C[0x89 - 0x5C];
    u8 range_89;
} Object;

typedef struct {
    s32 x;
    s32 y;
} Pos;

/* Action object built by func_801009C0 (0x14 bytes). */
typedef struct {
    short id_0;
    char pad2[2];
    s32 field_4;
    s32 field_8;
    const void *vtbl_C;
    void *owner_10;
} Action;

/* Command object built by func_800C4DA0 (0x14 bytes). */
typedef struct {
    void *owner_00;
    short value_04;
    s32 count_08;
    void *vtable_0C;
    void *link_10;
} Command;

extern Pos *func_800F13A0(Pos *ret, Object *unit, s32 range);
extern s32 func_800F1244(Object *arg, Object *target, s32 distance, s32 force);
extern Action *func_801009C0(Action *obj, void *owner);
extern Command *func_800C4DA0(Command *obj, void *owner, void *link, u16 value);
extern void func_800C4864(Command *cmd, s32 message_id, void *position);

/* Monster +0xB4 action slot (D_80159440 family): s32 (self, target). The caller-supplied
 * target is unused here; the attack goes at self->target_58. */
s32 func_80100C8C(Object *self, void *target) {
    Pos pos;

    func_800F13A0(&pos, self, self->range_89);
    switch (func_800F1244(self, self->target_58, self->range_89, 0)) {
    case 1:
        return 0;
    case 2:
        return 1;
    default: {
        Action action;
        Command command;

        func_801009C0(&action, self);
        func_800C4DA0(&command, self, &action, 1);
        func_800C4864(&command, 0x29, &pos);
        return 1;
    }
    }
}
