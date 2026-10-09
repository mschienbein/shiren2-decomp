#include "common.h"

typedef short s16;

/* Object table at obj+8, entry +0x38/+0x3C: message handler
 * s32 (void *receiver, void *event) (decided contract); only this entry is used here. */
typedef struct {
    short delta;
    short index;
    s32 (*func)(void *receiver, void *event);
} MessageEntry;

typedef struct {
    unsigned char pad0[0x38];
    MessageEntry message_38;
} VTable;

typedef struct {
    unsigned char unk0;
    unsigned char pad1[7];
    VTable *vtable;
} Obj;

typedef struct {
    s32 unk0;
    unsigned char pad4[0x14];
    s32 unk18;
    unsigned char pad1C[4];
} Message;

extern unsigned char D_80156D30[];
extern unsigned char D_80156D38[];
/* func_800AB4B0 returns a full int (addiu v0,v0,-1 after its sign extension at
 * 0x800AB520); this caller keeps the int and narrows it to signed char at each use
 * (sll at 0x800AB434 and sll/sra at 0x800AB458). */
extern s32 func_800AB4B0(void *table);
extern s32 func_800ACEB4(Obj *obj);
extern s32 func_8010BA90(Obj *obj, s16 value);
extern s32 func_8010B9F4(Obj *obj);

void func_800AB3F4(Obj *obj) {
    void *table = D_80156D38;
    s32 value;
    Message message;

    if (obj->unk0 == 3) {
        table = D_80156D30;
    }
    value = func_800AB4B0(table);
    if ((signed char)value < 0) {
        if (func_800ACEB4(obj) == 2) {
            value = 0;
        }
    }
    func_8010BA90(obj, (signed char)value);
    if ((s16)func_8010B9F4(obj) < 0) {
        Message *msg = &message;

        message.unk0 = 0x19;
        msg->unk18 = 1;
        obj->vtable->message_38.func((unsigned char *)obj + obj->vtable->message_38.delta, msg);
    }
}
