#include "common.h"
typedef struct { s32 x,y; } Coord;
typedef struct { unsigned char value; } Dir;
typedef struct { char pad0[0xC]; unsigned char field_c; char padd[0x13]; unsigned char field_20; } Obj;
extern s32 func_8010BEC4(void *, unsigned char);
extern void *func_800A2594(Coord *, void *, Dir);
extern u32 func_800B1C6C(Coord *);
/* ODD_C: this predicate helper preserves the original zero-test instruction. */
static inline s32 zero(s32 value) { return value==0; }
static inline s32 blocked_type(void *p) { return (unsigned char)func_8010BEC4(p,0x3A) || (unsigned char)func_8010BEC4(p,0x4D); }
static inline s32 passable(Coord *position) { return (func_800B1C6C(position)&0x4000) && zero(func_800B1C6C(position)&0x200); }
/* The caller's second pointer is not used by this collision test. */
s32 func_8010FC28(void *p, void *unused, Obj *target, void *position) {
    if(target->field_20) return 0;
    {
        s32 blocked=blocked_type(p);
        if(blocked) return 0;
        {
            unsigned char direction=target->field_c;
            if(direction&1) {
                Coord next,temp;
                Dir step;
                step.value = (direction + 1) & 7;
                func_800A2594(&next, position, step);
                if(passable(&next)) return 1;
                step.value = (target->field_c - 1) & 7;
                func_800A2594(&temp, position, step);
                next=temp;
                if(passable(&next)) return 1;
            }
        }
    }
    return 0;
}
