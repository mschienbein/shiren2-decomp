#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { s32 x; s32 y; } Position;
typedef struct {
    void *field_0;
    u32 field_4;
    u32 field_8;
    u16 field_C;
    u16 field_E;
    u8 field_10;
} Damage;

extern u32 D_8013960C;
void func_80136910(Damage *obj, void *a, u32 c, u32 b, u32 e);
s32 func_80049CB4(s32 id, ...);
void func_800A7B68(void *object, Damage *damage);
void *func_800AC244(u8 id);
s32 func_800ADC90(void *object, Position *position, void *origin);

static inline void copyPosition(Position *out, Position *in) {
    out->x = in->x;
    out->y = in->y;
}

static inline void strike(void *object) {
    Damage damage;

    func_80136910(&damage, 0, 1, 0x28, 0);
    func_80049CB4(0x1070, object);
    D_8013960C <<= 1;
    func_800A7B68(object, &damage);
    D_8013960C >>= 1;
}

/* actor: unused; func_8012408C forwards its actor pointer (0x80124114) and
 * func_800E1594 passes a null actor (0x800E1664). */
void func_800E4734(void *object, void *actor, s32 id) {
    Position copy;
    u8 index = id;
    void *target;

    copyPosition(&copy, object);
    strike(object);
    func_80049CB4(0x109, &copy);
    target = func_800AC244(index);
    if (target != 0) {
        func_800ADC90(target, &copy, &copy);
    }
}
