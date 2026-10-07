#include "common.h"

typedef short s16;
typedef unsigned char u8;
typedef struct Record_8012A7C4 Record_8012A7C4;

typedef struct {
    u8 *data;
    s32 priority;
} Entry;
/* GNU trailing payload: the bank's 0x18-byte header is followed by
 * eight-byte {sequence data, default priority} entries. func_8012C3E0
 * reads each priority at +0x1C + idx * 8 in the same backing bank. */
typedef struct {
    unsigned char pad0[0x10];
    Record_8012A7C4 *field_10;
    s32 field_14;
    Entry entries[0];
} Def;
typedef struct {
    unsigned char pad0[4];
    u8 *cursor_4;
    unsigned char pad8[0x3C];
    s32 id_44;
    s32 field_48;
    unsigned char pad4C[0x2C];
    Def *def_78;
    Record_8012A7C4 *record_7C;
    u8 *base_80;
    unsigned char pad84[0x1A];
    s16 field_9E;
    unsigned char padA0[6];
    s16 field_A6;
    unsigned char padA8[8];
    s16 field_B0;
} Obj;
extern void func_8012C004(Obj *);
extern s32 D_801CA6F0;

s32 func_8012C330(Obj *o, Def *def, s32 idx, s16 a3, s16 a4, s32 a5) {
    func_8012C004(o);
    o->field_A6 = idx;
    o->def_78 = def;
    o->field_9E = a3;
    o->field_B0 = a4;
    o->id_44 = D_801CA6F0++;
    o->field_48 = a5;
    if (def->field_10 != 0) {
        o->record_7C = def->field_10;
    }
    o->cursor_4 = o->base_80 = def->entries[idx].data;
    return o->id_44;
}
