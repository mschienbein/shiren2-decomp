#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    void (*func)(void *);
    u16 state;
    u16 parent;
    char pad8[0xE - 0x8];
    u16 unkE;
    union {
        s32 word;
        struct {
            u16 group;
            u16 flags;
        } half;
    } unk10;
    s32 unk14;
    char pad18[0x24 - 0x18];
    s32 unk24;
    char pad28[0x74 - 0x28];
} Task80085A80;

typedef struct {
    char pad0[0x7];
    u8 unk7;
} Handle80085A80;

extern Task80085A80 D_801BA380[];
extern void func_80085C24(void *task);
extern void func_8008C478(s32 index, u8 on);
extern void func_8008C194(s32 arg0);
extern Handle80085A80 *func_8007946C(s32 arg0, s32 arg1);
extern void func_80079560(s32 arg0, s32 arg1, s32 arg2);

void func_80085A80(Task80085A80 *task) {
    Task80085A80 *other;
    Handle80085A80 *handle;
    s32 i;

    if (task->parent != 0 && D_801BA380[task->parent - 1].state != 4 &&
        !(D_801BA380[task->parent - 1].unkE & 1)) {
        return;
    }
    task->state = 4;
    if (!(task->unk10.half.flags & 0x400)) {
        return;
    }
    for (i = task->parent + 1; i < 0xAE; i++) {
        other = &D_801BA380[i];
        if (other->func != func_80085C24) {
            continue;
        }
        if (other->parent == task->parent) {
            continue;
        }
        if (other->unk10.half.group != task->unk10.half.group) {
            continue;
        }
        if (!(other->unk10.half.flags & 0x800)) {
            continue;
        }
        func_8008C478(other->unk14, 0);
        func_8008C194(other->unk14);
        if ((other->unk10.word & 0xC) != 4 && !(other->unk10.half.flags & 0x80)) {
            handle = func_8007946C(0, other->unk24);
            func_80079560(0, other->unk24, 0);
            handle->unk7 = 1;
        }
        other->state = 4;
    }
}
