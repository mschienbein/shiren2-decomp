#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct Entry800EB7C4 Entry800EB7C4;
typedef struct List800EB7C4 List800EB7C4;

typedef struct {
    s32 index;
    List800EB7C4 *list;
    s32 reverse;
    Entry800EB7C4 *current;
} Iter800EB7C4;

struct Entry800EB7C4 {
    u8 type;
    u8 pad1[4];
    s8 unk5;
};

/* func_800EB120 embeds the list built by func_800CEC90 at offset 0xCC. */
struct List800EB7C4 {
    void *context;
    void *vtable;
    u8 *items;
    u8 field_C;
    u8 field_D;
    u8 field_E;
    void *owner;
    u16 label;
};

typedef struct {
    u8 pad0[0xCC];
    List800EB7C4 listCC;
} Obj800EB7C4;

void *func_800CEB20(Iter800EB7C4 *it, List800EB7C4 *list);
s32 func_800CEBA0(Iter800EB7C4 *it);
Entry800EB7C4 *func_800CEC68(Iter800EB7C4 *it);

Entry800EB7C4 *func_800EB7C4(Obj800EB7C4 *obj) {
    Iter800EB7C4 it;

    func_800CEB20(&it, &obj->listCC);
    while (func_800CEBA0(&it)) {
        Entry800EB7C4 *e = func_800CEC68(&it);
        if (e->type == 0xD && ~e->unk5 == 0) {
            return e;
        }
    }
    return 0;
}
