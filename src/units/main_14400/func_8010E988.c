#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct Unit Unit;
typedef struct Entity Entity;
typedef struct { void *field_00; u32 field_04; u32 field_08; u16 field_0C; u16 field_0E; u8 field_10; } Obj80136910;
typedef Obj80136910 Damage;
typedef struct { u8 pad_00[4]; void *field_04; Entity *field_08; u8 pad_0C[0x10]; Unit *field_1C; } Event;
extern s32 func_8010E5FC(void *arg0, Unit *unit);
extern void func_80136910(Obj80136910 *obj, void *a, u32 c, u32 b, u32 e);
extern void func_800A7ADC(Entity *, Damage *);

void func_8010E988(void *object, Event *event) {
    Damage damage;
    void *owner = event->field_04;
    s32 amount = func_8010E5FC(object, event->field_1C);
    Damage *output = &damage;
    func_80136910(output, owner, (s16)amount, 6, 0);
    func_800A7ADC(event->field_08, output);
}
