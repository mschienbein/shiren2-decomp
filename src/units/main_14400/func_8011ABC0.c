#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 x;
    s32 y;
} Pos8011ABC0;

typedef struct VTable8011ABC0 VTable8011ABC0;

typedef struct {
    char pad0[4];
    VTable8011ABC0 *vtbl;
} Sub8011ABC0;

struct VTable8011ABC0 {
    char pad0[0x10];
    short adjust10;
    s32 (*func14)(void *self);
};

typedef struct {
    u8 kind;
    char pad1[0xC - 1];
    Sub8011ABC0 sub;
} Obj8011ABC0;

char *func_800AE674(Obj8011ABC0 *obj);
void func_80114268(Obj8011ABC0 *obj, s32 arg1);
s32 func_80049CB4(s32 id, ...);
void func_800498E4(s32 id, ...);

/* Item-effect slot +0x44 supplies self, actor and item; this override ignores self. */
void func_8011ABC0(void *arg0, Pos8011ABC0 *pos, Obj8011ABC0 *obj) {
    if (obj != 0 && obj->kind == 9) {
        Sub8011ABC0 *sub = &obj->sub;
        u32 before = sub->vtbl->func14((char *)sub + sub->vtbl->adjust10);
        char *value = func_800AE674(obj);

        func_80114268(obj, 1);
        if (before < sub->vtbl->func14((char *)sub + sub->vtbl->adjust10)) {
            func_80049CB4(0x2D, pos);
            func_800498E4(0xD6, value);
        } else {
            Pos8011ABC0 copy;
            Pos8011ABC0 *p = &copy;

            p->x = pos->x;
            p->y = pos->y;
            func_80049CB4(0x11D, p);
            func_800498E4(0xD7, value);
        }
    } else {
        func_80049CB4(0x132);
        func_800498E4(0x222);
    }
}
