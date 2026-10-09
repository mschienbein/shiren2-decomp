#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Pos;
typedef struct { unsigned int prefix:8; unsigned int flag:1; unsigned int rest:23; } Flags;
typedef struct { u8 pad_00[0x20]; Flags flags_20; } Target;
extern u8 D_80156A3F;
extern short D_80156A34;
extern u8 D_80147620[];
extern s32 func_800C587C(void *rng, u8 limit);
extern s32 func_800A692C(void *self, s32 kind);
extern Pos func_800A6B70(u8 *src, u8 mode, s32 arg);
extern void *func_800B4928(Pos *pos);
extern s32 func_800A5758(void *self, void *target, short kind, unsigned short arg);
static inline s32 position_nonzero(Pos *pos) { return pos->y | pos->x; }
static inline Flags *get_flags(Flags *out, Target *target) { *out = target->flags_20; return out; }
static inline s32 is_allowed(Flags *flags) { return flags->flag != 1; }
void func_800F58EC(void *self) {
    Pos pos;
    Flags flags;
    Target *target;
    s32 allowed = 0;
    if (func_800C587C(D_80147620, D_80156A3F)) allowed = !func_800A692C(self, 0x12);
    if (allowed) {
        pos = func_800A6B70(self, 0x7C, 0);
        if (position_nonzero(&pos)) {
            target = func_800B4928(&pos);
            if (is_allowed(get_flags(&flags, target))) func_800A5758(self, target, D_80156A34, 0);
        }
    }
}
