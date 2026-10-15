#include "common.h"
#include "item_records.h"


typedef struct {
    s32 fields_00[24];
    short adjust_60;
    short field_62;
    void (*method_64)(void *);
} ActorTable;

typedef struct {
    unsigned char fields_00[0x1E];
    unsigned char flags_1E;
    unsigned char field_1F;
    s32 field_20;
    ActorTable *table_24;
} Actor;

typedef struct {
    s32 field_00;
    Actor *actor_04;
    s32 fields_08[4];
    s32 enabled_18;
    s32 forced_1C;
} Request;

extern s32 D_8013960C;
extern s32 func_8010BF6C(Item *, Actor *, s32);
extern void func_800ACD34(Item *);
extern s32 func_80049CB4(s32, ...);
extern void func_800498E4(s32, ...);
extern s32 func_8010EBA4(Item *);
extern s32 func_800AE498(Item *);

static inline s32 active_mask(Item *item) {
    return item->flags02 & 4;
}

void func_801104D0(Item *item, Request *request) {
    Actor *actor = request->actor_04;
    s32 enabled = request->enabled_18 != 0;
    s32 forced = request->forced_1C != 0;
    s32 special;
    s32 special_enabled = D_8013960C & 1;
    special = special_enabled;
    if (enabled != (active_mask(item) != 0)) {
        if (enabled) {
            if ((func_8010BF6C(item, actor, forced) ^ 1) != 0) {
                return;
            }
            item->flags02 |= 4;
            if ((actor->flags_1E >> 2) & 1) {
                func_800ACD34(item);
            }
            if (special_enabled) {
                ItemMethods *table;
                func_80049CB4(0x1129, 3);
                func_80049CB4(0x31, actor, item);
                func_800498E4(0x2B, func_800AE674(item));
                table = item->field08;
                if (table->test14((unsigned char *)item + table->delta10)) {
                    func_80049CB4(0x12B);
                    func_800498E4(0x2E, func_800AE674(item));
                }
            } else {
                func_80049CB4(0x84, actor, item);
            }
            if (func_8010EBA4(item)) {
                func_80049CB4(0x8D, actor, item);
            }
        } else {
            if (forced && (func_800AE498(item) ^ 1) != 0) {
                return;
            }
            item->payload.seals.field21 = 0;
            item->flags02 &= 0xFB;
            if (special) {
                func_80049CB4(0x1129, 3);
                func_80049CB4(0x32, actor, item);
                func_800498E4(0x2C, func_800AE674(item));
            } else {
                func_80049CB4(0x85, actor, item);
            }
        }
        if (actor->flags_1E & 0xC) {
            ActorTable *table = actor->table_24;
            table->method_64((unsigned char *)actor + table->adjust_60);
        }
    }
}
