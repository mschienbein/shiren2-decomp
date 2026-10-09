#include "common.h"

typedef unsigned char u8;
typedef struct { u8 field_00, field_01; u8 pad_02[10]; u32 field_0C; } Item;
typedef struct { u8 pad_00[0x20]; short delta_20; short index_22; s32 (*count)(void *); u8 pad_28[0x10]; short delta_38; short index_3A; Item *(*get)(void *, u32); } Methods;
/* Content list: record-table pointer +0 (func_800CE7A0), method table +4. */
typedef struct { void *field_00; Methods *field_04; } Container;
typedef struct { u8 pad_00[0x28]; Container *field_28[3]; u8 field_34[5][3]; u8 field_43[5][4]; signed char field_57[5][4][5]; } Obj;
static inline Container *container_at(char *cursor)
{
    return *(Container **)(cursor + 0x28);
}
void func_800D11C0(Obj *obj)
{
    s32 clear_index;
    char *cursor;
    char *end;
    s32 group, category, index;
    s32 counted_kind;
    for (clear_index = 0; clear_index < 5; clear_index++) {
        obj->field_34[clear_index][2] = 0;
        obj->field_34[clear_index][1] = 0;
        obj->field_34[clear_index][0] = 0;
    }
    counted_kind = 0x11;
    cursor = (char *)obj;
    end = cursor + sizeof(obj->field_28);
    /* local-arithmetic-qualification: both cursors remain in the owner;
     * the original compares their signed N64 addresses, unlike C pointer <. */
    for (;;) {
        Container *container;
        s32 more = (s32)cursor < (s32)end;
        if (!more) {
            break;
        }
        container = container_at(cursor);
        if (container != 0) {
            char *current = cursor;
            s32 item_index;
            item_index = container->field_04->count((u8 *)container + container->field_04->delta_20) - 1;
            for (;;) {
                Item *item;
                Container *owner;
                if (item_index < 0) {
                    break;
                }
                owner = container_at(current);
                item = owner->field_04->get((u8 *)owner + owner->field_04->delta_38, (u32)item_index);
                item_index--;
                if (item->field_00 == counted_kind) {
                    obj->field_34[item->field_01 - 0xE9][item->field_0C - 1]++;
                }
            }
        }
        cursor += sizeof(Container *);
    }
    group = 0;
    for (;;) {
        s32 more = group < 5;
        if (!more) {
            break;
        }
        category = 0;
        for (;;) {
            s32 more = category < 4;
            if (!more) {
                break;
            }
            obj->field_43[group][category] = 0;
            for (index = 0; index < 5; index++) {
                obj->field_57[group][category][index] = -1;
            }
            category++;
        }
        group++;
    }
}
