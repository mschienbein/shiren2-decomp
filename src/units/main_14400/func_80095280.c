#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { u8 kind; u8 type; } Item;
typedef struct { s32 unk0; u8 unk4[0x10]; Item *main; Item *sub; } Obj;
void func_80048764(void *arg0);
void func_80051860(Obj *obj, Item *item, s32 slot);
void func_8005197C(Obj *obj, Item *item, s32 slot);
char *func_80048480(u16 id);
void func_800487EC(void *obj, s32 row, s32 col, char *text);
void func_80095280(Obj *obj) {
    Item *item;
    s32 id;
    func_80048764(obj->unk4);
    func_80051860(obj, obj->main, 0);
    item = obj->main;
    if (item != 0 && obj->sub != 0 && item->type != 0x6D && item->kind != obj->sub->kind) {
        item = 0;
    }
    func_8005197C(obj, item, 0);
    id = 0x29E;
    if (obj->sub != 0) {
        if (obj->sub->kind == 3) {
            id = 0x29C;
        } else if (obj->sub->kind == 4) {
            id = 0x29D;
        }
    } else if (obj->main != 0) {
        if (obj->main->kind == 3) {
            id = 0x29C;
        } else if (obj->main->kind == 4) {
            id = 0x29D;
        }
    }
    func_800487EC(obj->unk4, 1, 2, func_80048480(id));
    func_80051860(obj, obj->sub, 2);
    func_8005197C(obj, obj->sub, 2);
}
