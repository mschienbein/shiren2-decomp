#include "common.h"
typedef struct { unsigned char fields00[0x4C]; const void *field4C; } Base;
/* Five complete base subobjects establish a minimum containing extent of 0x344. */
typedef struct {
    Base base;
    unsigned char pad50[0x7C];
    Base partCC;
    unsigned char pad11C[0x54];
    Base part170;
    unsigned char pad1C0[0xC];
    Base part1CC;
    unsigned char pad21C[0x18];
    s32 field234;
    const void *field238;
    unsigned char pad23C[0xB8];
    Base part2F4;
} Object;
extern const unsigned char D_80152A50[144];
extern const unsigned char D_801521D0[144];
extern const unsigned char D_80152968[152];
extern const unsigned char D_80152AE8[144];
extern const s32 D_80151EC8[]; /* Address-only view of the 0x14-byte callback table. */
extern Base *func_800953C0(Base *object);
Object *func_8009D4B0(Object *object) {
    Base *partCC;
    Base *part170;
    Base *part1CC;
    Base *part2F4;
    func_800953C0(&object->base);
    partCC = &object->partCC;
    object->base.field4C = D_80152A50;
    func_800953C0(partCC);
    part170 = &object->part170;
    partCC->field4C = D_801521D0;
    func_800953C0(part170);
    part1CC = &object->part1CC;
    part170->field4C = D_80152968;
    func_800953C0(part1CC);
    part2F4 = &object->part2F4;
    part1CC->field4C = D_80152AE8;
    object->field234 = -1;
    object->field238 = &D_80151EC8;
    func_800953C0(part2F4);
    part2F4->field4C = D_801521D0;
    return object;
}
