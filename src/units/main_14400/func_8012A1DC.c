#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct Record_8012A7C4 Record_8012A7C4;

typedef struct DefEntry {
    u8 *data_0;
    s32 value_4;
} DefEntry;

/* Sound definition: 0x18-byte header followed by eight-byte entries (GNU trailing array). */
typedef struct Def {
    u8 pad_00[0x10];
    Record_8012A7C4 *field_10;
    s32 field_14;
    DefEntry entries[0];
} Def;

/* 0x13C-byte voice record (see func_8012C330). */
typedef struct Voice8012A1DC {
    u8 pad_00[0x78];
    Def *def_78;
    u8 pad_7C[0xA6 - 0x7C];
    u16 index_A6;
    u8 pad_A8[0x13C - 0xA8];
} Voice8012A1DC;

extern Def *D_801CA70C;
extern Def *D_801CA708;
extern Record_8012A7C4 *D_801CA6F8;
extern s32 D_801CA6D4;
extern Voice8012A1DC *D_801CA6E0;

/* Values are forwarded at full int width; func_8012C330 narrows only at its stores. */
s32 func_8012C330(Voice8012A1DC *o, Def *def, s32 idx, s32 a3, s32 a4, s32 a5);
s32 func_8012C3E0(Def *obj, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

s32 func_8012A1DC(s32 index, s32 arg1, s32 arg2, s32 reuse, s32 arg4) {
    Def *def;
    s32 result;

    if (D_801CA70C != 0) {
        def = D_801CA70C;
        D_801CA70C = 0;
    } else {
        def = D_801CA708;
        if (def == 0) {
            D_801CA6F8 = 0;
            return 0;
        }
    }
    if (D_801CA6F8 == 0) {
        D_801CA6F8 = def->field_10;
    }
    if (reuse != 0) {
        Voice8012A1DC *voice = D_801CA6E0;
        s32 i;

        for (i = 4; i < D_801CA6D4; i++, voice++) {
            if (voice->index_A6 == index && voice->def_78 == def) {
                if (arg4 == -1) {
                    arg4 = def->entries[index].value_4;
                }
                result = func_8012C330(voice, def, index, arg1, arg2, arg4);
                D_801CA6F8 = 0;
                return result;
            }
        }
    }
    result = func_8012C3E0(def, index, arg1, arg2, arg4);
    D_801CA6F8 = 0;
    return result;
}
