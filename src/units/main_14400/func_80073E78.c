#include "common.h"

typedef float f32;
typedef struct { char pad[0x2C]; f32 f2C; f32 f30; char pad34[0x10]; s32 f44; s32 f48; } Obj;
typedef struct { s32 pos; f32 timer; } State;
typedef struct { u32 flags; short *data; } Script;
s32 func_80073E78(Obj *obj, State *st, Script *sc, f32 dt){
    s32 done = 0;
    short *data;
    u32 flags;
    if (st->timer != 0.0f) {
        st->timer -= dt;
        if (st->timer < 0.0f) st->timer = 0.0f;
    }
    if (st->timer != 0.0f) return done;
    data = sc->data;
    flags = sc->flags;
    st->timer = data[st->pos++];
    if (flags & 1) obj->f44 = data[st->pos++];
    if (flags & 2) obj->f48 = data[st->pos++];
    if (flags & 4) obj->f2C += data[st->pos++];
    if (flags & 8) obj->f30 += data[st->pos++];
    switch (data[st->pos]) {
    case -1: st->pos = 0; break;
    case -2: st->pos = data[st->pos + 1]; break;
    case -3: st->pos += data[st->pos + 1]; break;
    case -4: done = 1; break;
    }
    return done;
}
