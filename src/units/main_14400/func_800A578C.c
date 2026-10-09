#include "common.h"
typedef unsigned short u16;
typedef short s16;
typedef unsigned char u8;
typedef struct { s32 x; s32 y; } Point;
typedef struct { u32 value; } Flags;
typedef struct { Point point; char pad8[0x14]; u16 field1C; u8 field1E; u8 field1F; Flags field20; } Obj;
typedef struct { u8 pad[3]; u8 field3; } Tile;
typedef struct { s32 fields[6]; } Event;
extern Obj *D_801476B8;
extern u16 func_800B5768(Point *);
extern s32 func_800A58B8(Obj *);
extern s32 func_80049CB4(s32, ...);
extern char *func_800A3B20(Obj *);
extern void func_800497F0(s32, ...);
extern Tile *func_800B51D4(Point *);
extern void func_80136910(Event *, Obj *, u32, u32, u32);
extern void func_800A7ADC(Obj *, Event *);
static inline void copy_point(Point *out, Point *in) { out->x = in->x; out->y = in->y; }
static inline void create_event(Event *out, Obj *owner, s16 id) { func_80136910(out, owner, id, 34, 0x808); }
static inline Flags *copy_flags(Flags *out, const Flags *in) { *out = *in; return out; }
void func_800A578C(Obj *p) {
    Point point; Event event; Flags snapshot; s32 active = 0;
    if (!(p->field1C & 1)) { u32 bits = copy_flags(&snapshot, &p->field20)->value; s32 bit = (bits >> 25) & 1; active = bit == 0; }
    if (active) { s16 id; copy_point(&point, &p->point); id = (s16)func_800B5768(&point); if (id && func_800A58B8(p) >= 2) {
        s32 message = func_80049CB4(114, p); Tile *tile; Obj *owner;
        if (p->field1E & 0x7C) { func_80049CB4(6); func_800497F0(259, message, func_800A3B20(p)); func_80049CB4(7); }
        tile = func_800B51D4(&point); owner = 0; if (tile && tile->field3) owner = D_801476B8;
        create_event(&event, owner, id); func_800A7ADC(p, &event);
    } }
}
