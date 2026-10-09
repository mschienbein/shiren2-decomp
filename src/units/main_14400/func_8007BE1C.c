#include "common.h"
typedef unsigned char u8;
typedef short s16;
typedef struct { u8 active; u8 pad1[3]; s32 first, second; } Entry;
typedef struct { u8 pad0[0xE]; s16 fieldE; } Unit;
extern Entry D_801A75F0[16];
extern s32 func_8007920C(s32 kind, s32 id, s32 x, s32 y);
extern Unit *func_8007946C(s32 side, s32 slot);
extern s32 func_80076EBC(s32 x, s32 y);

s32 func_8007BE1C(s32 x, s32 y)
{
    s32 index;
    for (index = 0; index < 16; ++index) {
        Entry *entry = &D_801A75F0[index];
        if (!entry->active) {
            s32 id;
            Unit *unit;
            entry->active = 1;
            id = func_8007920C(0, 0x162, x, y);
            if (id != -1) {
                entry->first = id;
                unit = func_8007946C(4, id);
                unit->fieldE = func_80076EBC(x, y) * 4;
            } else {
                entry->first = id;
            }
            id = func_8007920C(0, 0x13A, x, y);
            if (id != -1) {
                entry->second = id;
                unit = func_8007946C(4, id);
                unit->fieldE = (func_80076EBC(x, y) + 1) * 4;
            } else {
                entry->second = id;
            }
            return index;
        }
    }
    return -1;
}
