#include "common.h"
#include "item_records.h"

typedef unsigned char u8;

typedef struct {
    s32 x;
    s32 y;
} Pos8011ABC0;

void func_80114268(Item *obj, s32 arg1);
s32 func_80049CB4(s32 id, ...);
void func_800498E4(s32 id, ...);

/* Item-effect slot +0x44 supplies self, actor and item; this override ignores self. */
void func_8011ABC0(void *arg0, Pos8011ABC0 *pos, Item *obj) {
    if (obj != 0 && obj->field00 == 9) {
        ItemList *sub = &obj->payload.pot.list;
        u32 before = sub->methods->limit14((char *)sub + sub->methods->delta10);
        char *value = func_800AE674(obj);

        func_80114268(obj, 1);
        if (before < sub->methods->limit14((char *)sub + sub->methods->delta10)) {
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
