#include "common.h"

typedef struct { void *field_00; s32 field_04; u32 field_08; } S;
typedef struct { unsigned char pad_00[0x28]; short delta_28; short index_2A; void (*method_2C)(void *, s32, void *); } Methods;
typedef struct { unsigned char pad_00[0x18]; Methods *field_18; } Obj;
extern const char D_80154880[];
extern unsigned char D_80148100[0x8E], D_801480F4[9], D_80148190[0xA2];
extern void func_800CA4E8(Obj *obj, void *message);
extern void *func_800A09B0(S *s, void *buf, u32 size);
extern void func_800A0B74(S *stream, unsigned char *out, u32 count);
void func_800D83F0(Obj *obj)
{
    S stream;
    s32 i;
    unsigned char *out;
    func_800CA4E8(obj, (void *)D_80154880);
    obj->field_18->method_2C((unsigned char *)obj + obj->field_18->delta_28, 0x8E, D_80148100);
    obj->field_18->method_2C((unsigned char *)obj + obj->field_18->delta_28, 9, D_801480F4);
    func_800A09B0(&stream, D_80148100, 0x8E);
    out = D_80148190;
    for (i = 0; i < 0xA2; i++) {
        func_800A0B74(&stream, out++, 7);
    }
}
