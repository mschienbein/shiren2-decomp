#include "common.h"
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;

typedef unsigned char u8;
typedef struct Message {
    s32 kind;
    void *target;
    u8 payload_08[0x10];
    s32 enabled_18;
    s32 field_1C;
} Message;
typedef struct VTable {
    u8 pad_00[0x38];
    short this_delta_38;
    short slot_3A;
    s32 (*message_3C)(void *, Message *);
} VTable;
typedef struct Object { u8 pad_00[8]; VTable *vtable_08; } Object;
typedef struct RandomState RandomState;

extern const u8 D_80156C88[];
extern RandomState D_80147620;
extern s32 func_800C587C(void *rng, u8 limit);

void func_800AB548(Object *object)
{
    Message message;
    if (func_800C587C(&D_80147620, D_80156C88[D_80142F24.index])) {
        Message *dispatch = &message;
        dispatch->kind = 0x19;
        dispatch->enabled_18 = 1;
        object->vtable_08->message_3C((u8 *)object + object->vtable_08->this_delta_38,
                                    dispatch);
    }
}
