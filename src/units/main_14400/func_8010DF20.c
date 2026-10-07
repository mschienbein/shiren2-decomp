#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

typedef struct { s32 x, y; } Vec2;
typedef struct { u8 x0; u8 x1; u8 x2; u8 x3; u8 x4; s8 x5; } Unit;
typedef struct { s32 type; u8 pad[0xC]; Vec2 pos; } Msg;
extern u32 func_800B1C6C(void *pos);
extern void *func_800A86EC(u8);
extern void func_800B4E7C(Vec2 *);
extern void func_800F4830(void *, Vec2 *);
extern void func_800498E4(s32 message_id, ...);
extern s32 func_800AF28C(Unit *, Msg *);
s32 func_8010DF20(Unit *u, Msg *m){
    if (m->type == 0x1A) {
        Vec2 pos;
        s32 ok = 0;
        void *obj;
        Vec2 *pp = &pos;
        pp->x = m->pos.x;
        pp->y = m->pos.y;
        if (!(func_800B1C6C(pp) & 0x2000) && !~u->x5) {
            s32 hidden = u->x2 & 0x40;
            ok = hidden == 0;
        }
        if (ok) {
            obj = func_800A86EC(u->x1 + 0x4F);
            if (obj) {
                func_800B4E7C(&pos);
                func_800F4830(obj, &pos);
            } else {
                func_800498E4(0x111);
            }
        }
        return 1;
    }
    return func_800AF28C(u, m);
}
