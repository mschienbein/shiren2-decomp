#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 kind;
    u8 id;
} Ent;

/* Word-aligned 16-byte stack storage; the constructor stores a collection pointer at +4. */
typedef struct { s32 index; void *collection; s32 active; Ent *current; } Iter;

typedef struct {
    u8 pad0[7];
    u8 flags_07;
    u8 pad8[0x18 - 0x8];
    s32 value_18;
} Status;

typedef struct {
    u8 pad0[0x90];
    s32 value_90;
    u8 flags_94;
    u8 bits_95[0xCC - 0x95];
    u8 list_CC[0xE4 - 0xCC];
    u16 flags_E4;
} Obj;

/* Bit-mask table; entry 3 selects the tested status bit. */
extern const u8 D_8015488C[8];

void func_800ECA18(Obj *obj);
Status *func_800C9E10(void);
Iter *func_800CEB20(Iter *it, void *list);
extern s32 func_800CEBA0(Iter *);
extern Ent *func_800CEC68(Iter *);
void *func_8011422C(u8 *);
void func_800EC9D0(Obj *obj, u8 id);
void func_800CF834(void *list);

void func_800EC864(Obj *obj, u8 kind, s32 refresh) {
    obj->flags_94 &= ~3;
    func_800ECA18(obj);
    obj->flags_94 &= ~4;
    if (kind == 0xB) {
        obj->flags_94 |= 6;
        obj->flags_E4 &= ~0x20;
    } else if (kind == 1 && !(func_800C9E10()->flags_07 & D_8015488C[3])) {
        Iter outer;
        Iter inner;

        obj->flags_94 |= 1;
        func_800CEB20(&outer, obj->list_CC);
        while (func_800CEBA0(&outer)) {
            Ent *ent = func_800CEC68(&outer);

            func_800EC9D0(obj, ent->id);
            if (ent->kind == 9) {
                func_800CEB20(&inner, func_8011422C((u8 *)ent));
                while (func_800CEBA0(&inner)) {
                    func_800EC9D0(obj, func_800CEC68(&inner)->id);
                }
            }
        }
    }
    if (refresh) {
        Status *status;

        func_800CF834(obj->list_CC);
        status = func_800C9E10();
        if (status != 0) {
            obj->value_90 = status->value_18;
        } else {
            obj->value_90 = 0;
        }
    }
}
