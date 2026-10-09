#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;
typedef struct { s32 field00; s32 field04; } Position;
typedef struct { unsigned char fields00[2]; unsigned char field02; } Item;
typedef struct { unsigned char fields00[0x20]; short field20; s32 (*field24)(void *); unsigned char fields28[0x10]; short field38; Item *(*field3C)(void *, u32); } CollectionMethods;
typedef struct { void *field00; CollectionMethods *field04; } Collection;
typedef struct { unsigned char fields00[0x98]; short field98; Collection *(*field9C)(void *); } ActorMethods;
typedef struct { Position position; unsigned char fields08[0x16]; unsigned char field1E; unsigned char fields1F[5]; ActorMethods *field24; } Actor;

extern unsigned char D_80147620[0x40];
extern s32 D_80148090;
extern unsigned short D_80156A0C;
extern s32 func_800A692C(Actor *actor, s32 code);
extern s32 func_800CF1C8(Collection *collection, unsigned char code);
extern s32 func_800C587C(void *random, unsigned char chance);
extern unsigned short func_800C58DC(void *random, unsigned short maximum);
extern s32 func_800AD714(Item *item, Position *position);
extern char *func_800AE674(Item *item);
extern void func_800CD304(Collection *collection, u32 index);
extern s32 func_800AD8AC(Item *item, Position *position);
extern void func_800498E4(s32 code, ...);
static inline void copy_position(Position *destination, Position *source) {
    destination->field00 = source->field00;
    destination->field04 = source->field04;
}
void func_800EACD4(Actor *actor) {
    Position position;
    Collection *collection;
    Item *item;
    s32 count;
    s32 index;
    unsigned char chance;
    char *value;
    s32 disabled = 0;
    if (((D_80142F18.flags >> 2) & 1) || D_80142F24.index == 9) {
        disabled = 1;
    }
    if (disabled == 0) {
        collection = actor->field24->field9C((unsigned char *)actor + actor->field24->field98);
        if (collection != 0 && func_800A692C(actor, 0x12) == 0) {
            count = collection->field04->field24((unsigned char *)collection + collection->field04->field20);
            if (count != 0) {
                chance = func_800CF1C8(collection, 0x7E) * D_80156A0C;
                if (chance != 0 && func_800C587C(&D_80147620, chance) != 0) {
                    index = func_800C58DC(&D_80147620, count - 1) & 0xFFFF;
                    item = collection->field04->field3C((unsigned char *)collection + collection->field04->field38, index);
                    if (item != 0 && !(item->field02 & 4)) {
                        Position *destination = &position;
                        copy_position(destination, &actor->position);
                        if (func_800AD714(item, destination)) {
                            value = func_800AE674(item);
                            func_800CD304(collection, index);
                            func_800AD8AC(item, destination);
                            func_800498E4(0x102, value);
                            if ((actor->field1E >> 2) & 1) {
                                D_80148090 = 0;
                            }
                        }
                    }
                }
            }
        }
    }
}
