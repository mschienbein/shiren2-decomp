#include "common.h"
typedef struct { s32 field0, field4; } Pair;
typedef struct { Pair field0; unsigned char pad8[0x20]; unsigned char field28, field29; } Object;
typedef struct { s32 words[6]; } Vector;
extern s32 func_80049CB4(s32, ...);
extern void *func_800AC5B4(s32, s32);
extern void *func_8010DE30(void *, s32);
extern s32 func_800AC670(void *);
extern char *func_800A3B20(Object *);
extern void func_800497F0(s32, ...);
extern void func_800A59A4(Object *);
extern void func_800AD7E0(void *, Pair *, s32);
extern void func_800ACD34(void *);
extern void func_80136910(Vector *, void *, u32, u32, u32);
extern void func_800A7B68(Object *, Vector *);
static inline Pair *copy_pair(Pair *out, Pair *source) {
    out->field0 = source->field0;
    out->field4 = source->field4;
    return out;
}
static inline Vector *zero_vector(Vector *out) {
    func_80136910(out, 0, 0, 0, 0);
    return out;
}
void func_800F48E0(Object *p) {
    Pair copy;
    Vector zero;
    s32 token = func_80049CB4(0xDA, copy_pair(&copy, &p->field0));
    void *item = func_8010DE30(func_800AC5B4(0xC, 0), p->field28 + 0xB3);
    s32 result = func_800AC670(item) ^ 1;
    if (result) {
        func_800497F0(0x10F, token, func_800A3B20(p));
        func_800A59A4(p);
        func_80049CB4(0x87, p);
        func_800AD7E0(item, &copy, 1);
        func_800ACD34(item);
        p->field29 = 0;
    } else {
        func_800497F0(0x110, token);
        func_800A7B68(p, zero_vector(&zero));
    }
}
