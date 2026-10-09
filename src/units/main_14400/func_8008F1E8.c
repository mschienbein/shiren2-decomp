#include "common.h"

typedef unsigned char u8;
typedef struct { char pad0[0x18]; u8 kind_18, flags_19, pad1A, mode_1B, mode_1C; char pad1D[3]; s32 field_20, field_24; u32 index_28; s32 field_2C, index_30, field_34, index_38; } Config;
typedef struct { char pad0[8]; void **values_8; char padC[0x10]; void **alternate_1C; } Table;
typedef struct { char pad0[0x74]; Table *table_74; } Owner;
typedef struct { u8 kind_0; char pad1[3]; u32 field_4, flags_8; s32 field_C, field_10; char pad14[0x1A0]; void *queue_1B4; } Object;
extern s32 func_8008D4B8(void *queue, void *value, s32 key);

s32 func_8008F1E8(Config *arg, Owner *owner, Object *obj) {
    s32 result;
    Config *config = arg;
    Object *view = obj;
    void *value;
    switch (config->kind_18) {
    case 0: obj->kind_0 = 1; break;
    case 1: obj->kind_0 = 0; break;
    }
    if (config->flags_19 & 1) view->flags_8 |= 0x200;
    if (config->flags_19 & 2) view->flags_8 |= 0x400;
    if (config->mode_1B == 2) view->flags_8 |= 1;
    switch (config->mode_1C) {
    case 1: view->field_4 = 0; break;
    case 2: view->field_4 = 0x100000; break;
    }
    view->field_C = config->field_20;
    view->field_10 = config->field_24;
    /* ODD_C: single-pass block stopping the dependent queue operations at the first error;
     * it also shapes the original saved-register choice (config s0, owner s1, object s2). */
    do {
        result = func_8008D4B8(obj->queue_1B4, owner->table_74->values_8[config->index_30], 1);
        if (result != 0) {
            break;
        }
        if (config->index_38 != -1) {
            result = func_8008D4B8(obj->queue_1B4, owner->table_74->values_8[config->index_38], 1);
            if (result != 0) {
                break;
            }
        }
        if (!(config->index_28 & 0x10000)) {
            value = owner->table_74->values_8[config->index_28];
        } else {
            value = owner->table_74->alternate_1C[config->index_28 & 0xFFFF];
        }
        result = func_8008D4B8(obj->queue_1B4, value, 0);
    } while (0);
    return result;
}
