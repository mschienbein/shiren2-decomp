#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_00[5]; signed char field_05; } Item;
typedef struct { void *field_00; Item *field_04; } Link;
typedef struct { u8 pad_00[0x20]; short adjust_20; short pad_22; s32 (*method_24)(void *); } ListMethods;
typedef struct { s32 field_00; const ListMethods *field_04; Link *field_08; s32 field_0C, field_10; } List;
typedef struct { u8 pad_00[0xB0]; List field_B0; s32 field_C4; } Object;
typedef struct { u8 pad_00[0x98]; short adjust_98; short pad_9A; void *(*method_9C)(void *); } ActorMethods;
typedef struct { u8 pad_00[0x24]; const ActorMethods *field_24; } Actor;
typedef struct { u8 pad_00[0x72]; u8 field_72; } ActorState;
extern Actor *D_801476B8;
extern s32 func_800EB820(Actor *actor);
extern s32 func_800EB8B0(s32 amount);
extern void *func_800A6D40(Actor *actor);
extern void func_800EB8C8(u8 *actor, s32 amount);
extern s32 func_800CD4C4(void *collection, void *item);
extern void *func_800D02AC(Link *link);
extern s32 func_800AE9AC(Item *item, s32 mode, s32 amount);
extern s32 func_800CD538(void *collection, Item *item);
extern void func_800EB744(Actor *actor, s32 amount);
extern s32 func_80049CB4(s32 id, ...);
static inline List *slots(Object *object) { return &object->field_B0; }
static inline s32 list_count(List *list) { return list->field_04->method_24((u8 *)list + list->field_04->adjust_20); }
s32 func_800D9578(Object *object) {
 s32 amount = 0;
 s32 rate = 0;
 s32 total;
 ActorState *state;
 void *inventory;
 List *list;
 s32 index;
 if (object->field_C4) { amount = func_800EB820(D_801476B8); rate = func_800EB8B0(amount); }
 total = 0;
 state = func_800A6D40(D_801476B8);
 func_800EB8C8((u8 *)D_801476B8, amount);
 inventory = D_801476B8->field_24->method_9C((u8 *)D_801476B8 + D_801476B8->field_24->adjust_98);
 index = list_count(&object->field_B0);
 list = slots(object);
 for (;;) {
  Link *link;
  Item *item;
  if (--index < 0) break;
  link = &list->field_08[index];
  item = link->field_04;
  if (func_800CD4C4(inventory, item)) {
   func_800D02AC(link);
   item->field_05 = -1;
   total += func_800AE9AC(item, 1, rate);
   func_800CD538(inventory, item);
  }
 }
 func_800EB744(D_801476B8, -total);
 func_80049CB4(0x89, D_801476B8);
 func_80049CB4(2);
 state->field_72 |= 4;
 return 0;
}
