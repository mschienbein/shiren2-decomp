#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
typedef struct { s32 x; s32 y; } Position;
typedef struct { s32 type; void *target; void *item; s32 fields[5]; } Event;
/* Entity table at +0x24: +0xC destructor, +0x9C contents accessor. */
typedef struct {
    u8 pad_00[8]; short delta_08; short pad_0A; void (*destroy_0C)(void *self, s32 flags);
    u8 pad_10[0x88]; short delta_98; short pad_9A; void *(*contents_9C)(void *self);
} EntityTable;
typedef struct Entity {
    Position position; u8 pad_08[0x14]; u16 field_1C; u8 pad_1E[6]; EntityTable *table_24;
    u8 pad_28[0x30]; struct Entity *target_58; u8 pad_5C[0x3E]; u16 field_9A;
} Entity;
typedef struct { u8 pad_00[0xC]; u8 flags_0C; } SubItem;
/* Collection table: +0x24 count, +0x3C get(index). */
typedef struct {
    u8 pad_00[0x20]; short delta_20; short pad_22; s32 (*count_24)(void *self);
    u8 pad_28[0x10]; short delta_38; short pad_3A; void *(*get_3C)(void *self, u32 index);
} CollectionTable;
typedef struct { s32 field_00; CollectionTable *table_04; } Collection;
/* Message table at +8: +0x3C handler. */
typedef struct { u8 pad_00[0x38]; short delta_38; short pad_3A; s32 (*handle_3C)(void *receiver, void *event); } ItemTable;
typedef struct { u8 kind; u8 type_01; u8 pad_02[6]; ItemTable *table_08; Collection contents_0C; } Item;
extern SelectionRecord D_80142F18;
extern u8 D_80147620[];
extern u32 D_8013960C;
extern void *D_801476B8;
extern u8 *D_80148400[];
void func_800FDBBC(Entity *unit);
s32 func_800E0F40(Entity *obj);
s32 func_800F069C(void *arg);
s32 func_800C5844(void *rng, u8 base, u8 top);
void func_800AA700(u8 *kind, u8 *level, u8 *table);
void *func_800A8694(u8 a, u8 b, void *mem);
s32 func_800A5D2C(void *object, Position *output, s32 flags);
s32 func_80049CB4(s32 id, ...);
void func_800A58FC(void *actor, Position *position);
void *func_800A65E4(u8 *direction, Entity *object, Entity *target);
void func_800A665C(Entity *obj, u8 *value);
s32 func_800A8FC8(s32 *iterator, s32 kind);
void *func_800A910C(s32 *iterator);
u16 func_800E08B0(Entity *state);
s32 func_800E20CC(void *arg0);
u8 func_800A6420(Entity *obj, Entity *target);
s32 func_800A8A50(void);
void *func_800CEAF0(void *iterator, void *collection, s32 mode);
s32 func_800CEBA0(s32 *iterator);
Item *func_800CEC68(s32 *iterator);
s32 func_800AC670(void *key);
void *func_8011422C(Item *obj);
void func_800CD304(void *obj, u32 value);
s32 func_800A4520(void *ctx, Entity *obj);
static inline s32 special_mode(void) { return D_80142F18.mode == 0x4F; }
static inline void copy_position(Position *destination, Position *source) {
    destination->x = source->x;
    destination->y = source->y;
}
static inline s32 eligible(Entity *root, Entity *object) {
    s32 result = 0;
    if ((object->field_9A & 0x40) && func_800E08B0(object) && !func_800E20CC(object)
        && !(object->field_1C & 1) && !(((u8 *)&object->field_9A)[0] & 1))
        result = func_800A6420(root, object) == 3;
    return result;
}
static inline s32 subitem_flag(SubItem *child) {
    return child->flags_0C & 8;
}
static inline void *collection_get(Collection *contents, u32 index) {
    return contents->table_04->get_3C((u8 *)contents + contents->table_04->delta_38, index);
}
/* Monster +0xB4 action override (D_8015A940 slot, decided s32 (self, target) contract): the
 * dispatcher supplies the func_800A6CF0 target, which this action does not read. */
s32 func_800FDC28(Entity *root, void *target_unused) {
    Position position;
    s32 iterator[4];
    Event event;
    u8 kind;
    u8 level;
    u8 first_direction;
    s32 scan;
    s32 scan_again;
    u8 direction;
    s32 started;
    s32 count;
    if (special_mode()) {
        func_800FDBBC(root);
        return 1;
    }
    level = func_800E0F40(root);
    count = 1;
    started = 0;
    if (func_800F069C(root)) {
        if (level == 3) count = (u8)func_800C5844(D_80147620, 2, 3);
        for (;;) {
            s32 previous_count = count--;
            Entity *object;
            if (previous_count <= 0) break;
            func_800AA700(&kind, &level, D_80148400[level - 1]);
            if (!kind) continue;
            object = func_800A8694(kind, func_800E0F40(root), 0);
            if (!object) continue;
            copy_position(&position, &root->position);
            if (func_800A5D2C(object, &position, 1)) {
                if (!started) {
                    func_800FDBBC(root);
                    started = 1;
                }
                func_80049CB4(6);
                func_80049CB4(0x82, object, &position);
                func_800A58FC(object, &position);
                func_800A65E4(&first_direction, object, root->target_58);
                func_800A665C(object, &first_direction);
                func_80049CB4(7);
            } else {
                object->table_24->destroy_0C((u8 *)object + object->table_24->delta_08, 3);
                break;
            }
        }
    } else {
        count = level;
        if ((u8)func_800E0F40(root) == 3) {
            s32 found = 0;
            scan = 0;
            for (;;) {
                s32 active = func_800A8FC8(&scan, 16);
                Entity *object;
                if (!active) break;
                object = func_800A910C(&scan);
                if (eligible(root, object)) found++;
            }
            D_8013960C <<= 1;
            if (found < count && func_800A8A50()) {
                Entity *owner = D_801476B8;
                void *collection = owner->table_24->contents_9C((u8 *)owner + owner->table_24->delta_98);
                func_800CEAF0(iterator, collection, 0);
                for (;;) {
                    s32 active = func_800CEBA0(iterator);
                    Item *item;
                    s32 populated;
                    Collection *contents;
                    SubItem *child;
                    s32 suitable;
                    if (!active) break;
                    item = func_800CEC68(iterator);
                    populated = 0;
                    if (item->type_01 == 0xAC) {
                        contents = &item->contents_0C;
                        populated = contents->table_04->count_24((u8 *)contents + contents->table_04->delta_20) != 0;
                    }
                    if (!populated) continue;
                    child = collection_get(&item->contents_0C, 0);
                    suitable = 0;
                    {
                        s32 available = func_800AC670(child) != 1;
                        if (available) suitable = !subitem_flag(child);
                    }
                    if (!suitable) continue;
                    if (!started) {
                        func_800FDBBC(root);
                        started = 1;
                    }
                    event.type = 12;
                    event.item = child;
                    event.target = D_801476B8;
                    if (item->table_08->handle_3C((u8 *)item + item->table_08->delta_38, &event)) {
                        func_800CD304(func_8011422C(item), 0);
                        found++;
                        if (found >= count) break;
                    }
                }
            }
            D_8013960C >>= 1;
            if (started) func_80049CB4(0x132);
        }
        for (;;) {
            s32 previous_count = count--;
            if (previous_count <= 0) break;
            scan_again = 0;
            for (;;) {
                s32 active = func_800A8FC8(&scan_again, 16);
                Entity *object;
                if (!active) break;
                object = func_800A910C(&scan_again);
                if (!eligible(root, object)) continue;
                if (func_800A4520(object, root->target_58)) object->target_58 = root->target_58;
                else object->target_58 = 0;
                copy_position(&position, &root->position);
                if (!func_800A5D2C(object, &position, 1)) continue;
                if (!started) func_800FDBBC(root);
                func_80049CB4(0x10A8, object);
                func_800A58FC(object, &position);
                func_80049CB4(20, object, &position);
                func_800A65E4(&direction, object, root->target_58);
                started = 1;
                func_800A665C(object, &direction);
                func_80049CB4(31, object);
                break;
            }
        }
    }
    func_80049CB4(0x132);
    if (!started && func_800E20CC(root)) {
        func_80049CB4(0x5C, root);
        return 1;
    }
    return started;
}
