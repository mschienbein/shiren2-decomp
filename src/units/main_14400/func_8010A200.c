#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x,y; } Pos;
typedef Pos Pair;
typedef Pos Value;
typedef struct { u8 value; } Dir;
typedef union { u32 word; struct { u32 unused_high:7; u32 water:1; u32 unused_low:24; } bits; } Flags;
typedef struct Object {
    Pos position;
    u8 direction_8;
    u8 pad_9[0x17];
    Flags flags_20;
    u8 pad_24[0x30];
    u8 flags_54;
    u8 pad_55[3];
    struct Object *target_58;
    u8 pad_5C[0x80];
    u8 mode_DC;
} Object;
typedef Object Obj;
typedef Object Obj800E8A68;
typedef Object Obj800A46BC;
typedef Object Unit800E7104;
typedef Object T;
typedef struct Target Target;
typedef Target S;
extern void *func_800E8A68(Obj800E8A68 *obj, u8 kind);
extern s32 func_8010BEC4(S *s, u8 c);
extern void *func_800A492C(void *a, s32 b, s32 c, s32 d);
extern s32 func_800A650C(Object *object, Value *value);
extern u32 func_800B1C6C(Pos *pos);
extern s32 func_800A4754(Obj800A46BC *obj, void *arg, Dir *cell);
extern s32 func_8010EC5C(Target *effect, Object *obj);
extern void *func_800A6538(void *out_direction, void *obj, void *target);
/* Explicit-output contract: the result buffer is the first argument (sp+0x20 in a0
 * at 0x8010A388..0x8010A398); the returned pointer is unused here. */
extern Pos *func_8010ECD0(Pos *out, Target *effect, Pos *position, Dir *direction);
extern s32 func_800A251C(Pair *x, Pair *y);
extern void *func_800A65E4(Dir *p, T *q, void *target);
extern void func_800A665C(Obj *obj, u8 *value);
extern s32 func_800E7104(Unit800E7104 *unit);
extern s32 func_800E776C(void *arg0, u8 arg1);
static __inline__ Flags *snapshot_flags(Flags *out, Object *self) {
    out->word=self->flags_20.word;
    return out;
}
s32 func_8010A200(Object *self, u8 mode) {
    Pos origin, destination, prediction;
    Flags flags;
    Dir facing, toward, direction;
    Object *target=self->target_58;
    Target *effect=func_800E8A68(self,3);
    s32 first, second;
    s32 stop;
    Pos *start;
    if (effect) first=(u8)func_8010BEC4(effect,0x78)!=0;
    else first=0;
    if (!first) first=(snapshot_flags(&flags,self)->word>>24)&1;
    if (effect) second=(u8)func_8010BEC4(effect,0x4E)!=0;
    else second=0;
    self->mode_DC=0;
    if (target==0) target=func_800A492C(self,2,1,second^1);
    if (target!=0) {
        start=&origin;
        start->x=self->position.x;
        start->y=self->position.y;
        facing.value=self->direction_8;
        destination.x=target->position.x;
        destination.y=target->position.y;
        stop=0;
        if (func_800A650C(self,&destination)==1) {
            s32 blocked=0;
            if ((first && (func_800B1C6C(&destination)&0x4000)) ||
                (second && func_800A4754(self,start,&facing))) blocked=1;
            if (blocked) stop=1;
        } else {
            s32 blocked=0;
            if (effect && func_8010EC5C(effect,self)) {
                func_800A6538(&toward,self,&destination);
                func_8010ECD0(&prediction,effect,start,&toward);
                blocked=func_800A251C(&prediction,&destination)!=0;
            }
            if (blocked) { stop=1; self->mode_DC=2; }
        }
        self->target_58=target;
        if (stop) {
            func_800A65E4(&direction,self,target);
            func_800A665C(self,&direction.value);
            self->flags_54|=4;
            return 0;
        }
        if (mode!=1) return func_800E7104(self);
    }
    return func_800E776C(self,3);
}
