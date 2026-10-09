#include "common.h"

typedef struct Item Item;
typedef struct {
    unsigned char unk00[0x38];
    short unk38;
    Item *(*unk3C)(void *, u32);
} OwnerTable;
typedef struct { s32 unk00; OwnerTable *unk04; } Owner;
typedef struct {
    unsigned char unk00[0x48];
    short unk48;
    unsigned char (*unk4C)(void *);
} ItemTable;
struct Item {
    unsigned char unk00;
    unsigned char unk01;
    unsigned char unk02[6];
    ItemTable *unk08;
    signed char unk0C;
    signed char unk0D;
};
typedef struct { Owner *unk00; Item *unk04; } Object;
extern s32 func_8009A2B8(void *object);
extern s32 func_800A0E1C(Object *, s32 *, unsigned short, s32 (*)(void *));

s32 func_800A0EF4(Object *object) {
    s32 selection[4];
    s32 status;
    Owner *owner;
    OwnerTable *owner_table;
    Item *item;
    ItemTable *item_table;
    Item *current;
    status = func_800A0E1C(object, selection, 0x1E8, func_8009A2B8);
    if (status != 1) {
        return status;
    }
    owner = object->unk00;
    owner_table = owner->unk04;
    item = owner_table->unk3C((unsigned char *)owner + owner_table->unk38, selection[0]);
    object->unk04 = item;
    item_table = item->unk08;
    status = item_table->unk4C((unsigned char *)item + item_table->unk48);
    current = object->unk04;
    if ((status & 0xFF) != current->unk01) {
        return 1;
    }
    if (current->unk0D >= 0x63) {
        return -2;
    }
    return 1;
}
