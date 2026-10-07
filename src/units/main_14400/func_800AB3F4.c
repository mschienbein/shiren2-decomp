#include "common.h"

typedef short s16;

typedef struct {
    short delta;
    short index;
    void (*func)(void *self, void *arg1);
} VtableEntry;

typedef struct {
    unsigned char unk0;
    unsigned char pad1[7];
    VtableEntry *vtable;
} Obj;

typedef struct {
    s32 unk0;
    unsigned char pad4[0x14];
    s32 unk18;
    unsigned char pad1C[4];
} Message;

extern unsigned char D_80156D30[];
extern unsigned char D_80156D38[];
extern signed char func_800AB4B0(void *table);
extern s32 func_800ACEB4(Obj *obj);
extern s32 func_8010BA90(Obj *obj, s16 value);
extern s32 func_8010B9F4(Obj *obj);

void func_800AB3F4(Obj *obj) {
    void *table = D_80156D38;
    signed char value;
    Message message;

    if (obj->unk0 == 3) {
        table = D_80156D30;
    }
    value = func_800AB4B0(table);
    if (value < 0) {
        if (func_800ACEB4(obj) == 2) {
            value = 0;
        }
    }
    func_8010BA90(obj, value);
    if ((s16)func_8010B9F4(obj) < 0) {
        Message *msg = &message;

        message.unk0 = 0x19;
        msg->unk18 = 1;
        obj->vtable[7].func((unsigned char *)obj + obj->vtable[7].delta, msg);
    }
}
