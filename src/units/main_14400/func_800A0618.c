#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct { void *field_0; u32 field_4; u32 field_8; s16 field_C; u16 field_E; u8 field_10; } Damage;
typedef struct { u8 pad_0[0x1E]; u8 field_1E; } Entity;
/* Pending damage request at 0x80142904 (same object as func_800A0600 and
 * func_800A00C4): attacker, kind and percentage. */
typedef struct { void *attacker; s32 kind; u8 percentage; } Request;
extern void func_80136910(Damage *obj, void *a, u32 c, u32 b, u32 e);
extern u16 func_800E08B0(void *obj);
extern void func_800A7ADC(Entity *, Damage *);
extern void func_800A7B68(Entity *, Damage *);
extern Request D_80142904;

static inline void *requestAttacker(Request *request) {
    return request->attacker;
}

static inline s32 requestKind(Request *request) {
    return request->kind;
}

void func_800A0618(Entity *object)
{
    Damage damage;
    if (D_80142904.percentage != 0) {
        Damage *payload = &damage;
        func_80136910(payload, requestAttacker(&D_80142904), 0, requestKind(&D_80142904), 0x408);
        if (object->field_1E & 12) {
            damage.field_C = (func_800E08B0(object) * D_80142904.percentage) / 100;
            if (damage.field_C == 0) {
                damage.field_C = 1;
            }
            func_800A7ADC(object, payload);
        } else {
            func_800A7B68(object, payload);
        }
    }
}
