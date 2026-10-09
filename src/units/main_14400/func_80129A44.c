#include "common.h"
typedef unsigned char u8;
typedef short s16;
typedef struct Def Def;
typedef struct Record_8012A7C4 Record_8012A7C4;
typedef struct { char pad0[0x44]; s32 field44; s32 field48; char pad4C[0x2C]; Def *field78; Record_8012A7C4 *field7C; char pad80[0x1E]; s16 field9E; char padA0[0x10]; s16 fieldB0; char padB2[0x8A]; } Obj;
extern s32 D_801CA6D4;
extern Obj *D_801CA6DC;
extern s32 func_8012C3E0(Def *, s32, s32, s32, s32);
static inline void relink(Obj *p, s32 found) { s32 count = D_801CA6D4; Obj *entry = D_801CA6DC; s32 i = 0; if (count > 0) { s32 limit = count; do { if (entry->field44 == found) { entry->field44 = p->field44; entry->field7C = p->field7C; } ++entry; ++i; } while (i < limit); } }
u8 *func_80129A44(Obj *p, u8 *data) {
    s32 id = *data++;
    s32 found;
    if (id >= 128) { s32 low = *data++; id = ((id & 127) << 8) + low; }
    ++p->field48;
    found = func_8012C3E0(p->field78, id, p->field9E, p->fieldB0, p->field48);
    --p->field48;
    if (found) relink(p, found);
    return data;
}
