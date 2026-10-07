#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad0[0x2CC]; s32 field_2CC; } Actor8009A814;
typedef struct { s32 field_0; Actor8009A814 *actor; s32 kind; } Entry8009A814;
typedef struct { u8 pad0[0x50]; Entry8009A814 *entries; } Obj8009A814;
extern Actor8009A814 D_801408EC;
extern s32 D_80140BB8;
Actor8009A814 *func_8009A814(Obj8009A814 *obj, s32 index) {
    Actor8009A814 *actor = obj->entries[index].actor;
    Entry8009A814 *entry = &obj->entries[index];
    if (actor == &D_801408EC) {
        switch (entry->kind) {
            case 0x16: case 0x19: D_80140BB8 = 0; break;
            case 0x15: actor->field_2CC = 1; break;
        }
    }
    return actor;
}
