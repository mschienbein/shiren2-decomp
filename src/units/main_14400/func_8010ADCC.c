#include "common.h"

typedef struct {
    s32 x;
    s32 y;
} Pos8010ADCC;

typedef struct {
    Pos8010ADCC pos;
    char pad8[0xC0 - 8];
    s32 unkC0;
    Pos8010ADCC unkC4;
} Obj8010ADCC;

extern Pos8010ADCC *D_801476B8;
void *func_800B4D80(Pos8010ADCC *pos);
s32 func_800A251C(Pos8010ADCC *pos, Pos8010ADCC *other);
void func_800AD868(Pos8010ADCC *pos);
s32 func_80049CB4(s32 id, ...);
s32 func_800ADC90(void *target, Pos8010ADCC *a, Pos8010ADCC *b);
s32 func_800EF210(Obj8010ADCC *obj);

s32 func_8010ADCC(Obj8010ADCC *obj) {
    Pos8010ADCC pos;
    Pos8010ADCC dest;
    Pos8010ADCC *p = &pos;
    void *target;

    p->x = obj->pos.x;
    p->y = obj->pos.y;
    if (obj->unkC0 != 0) {
        target = func_800B4D80(&obj->unkC4);
        if (target != 0 && func_800A251C(p, &obj->unkC4)) {
            dest.x = D_801476B8->x;
            dest.y = D_801476B8->y;
            func_800AD868(p);
            func_80049CB4(0x10E1, p);
            func_80049CB4(0x29, obj);
            func_80049CB4(0xC0, target, p, &dest);
            func_800ADC90(target, &dest, &dest);
            obj->unkC0 = 0;
            return 1;
        }
    }
    return func_800EF210(obj);
}
