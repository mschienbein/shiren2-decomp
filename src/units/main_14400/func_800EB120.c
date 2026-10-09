#include "common.h"
typedef struct { unsigned char pad[0xA]; unsigned char fieldA; unsigned char padB[0x13]; unsigned char field1E, field1F; s32 field20; const void *field24; unsigned char pad28[0xCC]; const void *fieldF4; } Object;
extern Object *func_800E8730(Object *);
extern void *func_800CEC90(void *obj, void *owner, void *entries, unsigned char capacity, unsigned short text_id);
extern void func_80136908(void *);
extern void func_800EB1A0(Object *);
extern const unsigned char D_80159008[192], D_801544C0[132];
Object *func_800EB120(Object *p) {
    func_800E8730(p);
    p->field24 = D_80159008;
    func_800CEC90((unsigned char *)p + 0xCC, p, (unsigned char *)p + 0xB8, 0x14, 0x467);
    func_80136908((unsigned char *)p + 0xE8);
    p->fieldF4 = D_801544C0;
    p->field1E = 4;
    p->fieldA = 0x17;
    p->field1F = 0x17;
    func_800EB1A0(p);
    return p;
}
