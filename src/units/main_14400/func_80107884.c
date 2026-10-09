#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct Object Object;
struct Object {
    u8 pad0[0x28]; u16 field28;
    u8 pad2A[0x48]; u8 field72;
    u8 pad73[0x27]; u16 field9A;
};
typedef struct {
    Object *source;
    u32 kind, field8;
    u16 fieldC, fieldE;
    u8 field10;
} Damage;
typedef struct { s32 kind; u8 pad4[0xC]; Damage *damage; } Message;
typedef struct { s32 index; Object *object; } Iterator;
extern u8 D_801531A0[];
extern u32 D_8013960C;
extern u16 D_8014767C;
extern s32 func_801F258C(s32, s32);
extern void func_800F16C0(Object *, s32);
extern void func_800E42AC(Object *, Damage *);
extern void func_80136910(Damage *, void *, u32, u32, u32);
extern s32 func_800A8FC8(Iterator *, s32);
extern void *func_800A910C(Iterator *);
extern u16 func_800E08B0(void *);
extern s32 func_80049CB4(s32, ...);
extern void func_800A7B68(Object *, Damage *);
extern s32 func_800F27A4(Object *, Message *);

s32 func_80107884(Object *self, Message *message)
{
    switch (message->kind) {
    case 10:
        switch (func_801F258C(0x53, 0) ^ 1) {
        case 0:
            {
                Damage damage;
                Iterator iterator;
                Iterator *current;
                Damage *incoming = message->damage;
                if (!(D_801531A0[incoming->kind] & 0x20)) {
                    func_800F16C0(self, incoming->kind == 0x27);
                }
                func_800E42AC(self, incoming);
                self->field72 |= 4;
                self->field28 = 1;
                D_8013960C <<= 1;
                func_80136910(&damage, 0, 0, 0, 0);
                current = &iterator;
                current->index = 0;
                while (func_800A8FC8(current, 0x10)) {
                    Object *object = func_800A910C(current);
                    s32 affected = func_800E08B0(object) && object != self &&
                        !(object->field9A & 0x40);
                    if (affected) {
                        func_80049CB4(6);
                        func_800A7B68(object, &damage);
                        func_80049CB4(7);
                    }
                }
                D_8013960C >>= 1;
                D_8014767C |= 0x40;
                return 1;
            }
        }
        break;
    case 0x15:
        return 0;
    }
    return func_800F27A4(self, message);
}
