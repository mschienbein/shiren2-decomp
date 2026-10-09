#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x; s32 y; } Pos;
typedef struct { u8 v; } Dir;
typedef struct { unsigned pad : 7; unsigned diagonalOk : 1; unsigned rest : 24; } Flags;
typedef struct { char pad[0x20]; Flags flags; } Actor;
void *func_800A2594(Pos *, void *, Dir);
u32 func_800B1C6C(Pos *);
s32 func_800A4360(Actor *, Pos *);
u8 func_800A5AA0(Actor *);
static inline s32 Flags_diagonalOk(Flags *f){ return f->diagonalOk; }
static inline Dir Dir_make(s32 v){ Dir d; d.v = v; return d; }
static inline s32 terrain_is_water(u32 t) { if (t & 0x2000) return 1; return 0; }
s32 func_800A4754(Actor *self, Pos *pos, Dir *dir){
    Pos tmp;
    s32 left, right, ok, r;
    Flags f;
    func_800A2594(&tmp, pos, Dir_make((dir->v + 1) & 7));
    left = func_800B1C6C(&tmp);
    func_800A2594(&tmp, pos, Dir_make((dir->v - 1) & 7));
    right = func_800B1C6C(&tmp);
    f = self->flags;
    if (Flags_diagonalOk(&f)) {
        if (dir->v & 1) {
            r = 0;
            if (!(left & 0x8020) || (left & 0x200)) {
                if (!(right & 0x8020) || (right & 0x200)) r = 1;
            }
            return r;
        }
    } else {
        func_800A2594(&tmp, pos, Dir_make(dir->v));
        ok = 0;
        if (func_800A4360(self, pos) == 1) ok = func_800A5AA0(self) == 1;
        if (ok) {
            Pos *t = &tmp;
            if (func_800B1C6C(t) & 0x4100) return 0;
            if (dir->v & 1) {
                if (func_800B1C6C(t) & 0x2000) {
                    return terrain_is_water(left) && terrain_is_water(right);
                }
                return (left & 0x4200) != 0x4000 && (right & 0x4200) != 0x4000;
            }
        } else {
            if (func_800B1C6C(&tmp) & 0x4000) return 0;
            if (dir->v & 1) return (left & 0x4200) != 0x4000 && (right & 0x4200) != 0x4000;
        }
    }
    return 1;
}
