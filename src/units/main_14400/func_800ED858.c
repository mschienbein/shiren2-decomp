#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad_0[0x88]; s32 hunger_88; u8 pad_8C[0x58]; u16 flags_E4; u8 pad_E6[6]; s32 previous_EC; } Obj800ED72C;
typedef Obj800ED72C Obj800E8A68;
typedef struct Object Object;
extern void *func_800E8A68(Obj800E8A68 *obj, u8 kind);
extern s32 func_8010CF00(Object *object, s32 kind, s32 value);
extern u16 func_800EB6A0(void *arg, u16 value);
extern void func_800A7B18(void *target, void *source, s32 amount, s32 kind);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800498E4(s32 id, ...);
extern s32 D_80148090;
s32 func_800ED858(Obj800ED72C *self) {
    s32 okay;
    s32 cost=100;
    u16 flags=self->flags_E4;
    u16 message;
    Object *effect;
    okay=1;
    if ((flags >> 2)&1) cost=0;
    else {
        effect=func_800E8A68(self,4);
        if (effect) cost=func_8010CF00(effect,2,100);
        cost=func_800EB6A0(self,cost);
    }
    if (self->hunger_88<=0 && (cost==0 || cost>100)) cost=100;
    if (cost!=0) {
        self->hunger_88-=cost;
        if (self->hunger_88 < -300) self->hunger_88=-300;
    }
    if (self->hunger_88<0 && self->previous_EC>0) self->hunger_88=0;
    message=0;
    if (self->hunger_88==-300) {
        func_800A7B18(self,0,1,0x23);
        okay=0;
    } else if (self->hunger_88 < -199 && self->previous_EC >= -199) {
        message=0x12; okay=0;
    } else if (self->hunger_88 < -99 && self->previous_EC >= -99) {
        message=0x11; okay=0;
    } else if (self->hunger_88<=0 && self->previous_EC>0) {
        message=0x10; okay=0;
    } else if (cost!=0) {
        if (self->hunger_88<10001 && self->previous_EC>=10001) message=0xF;
        else if (self->hunger_88<20001 && self->previous_EC>=20001) message=0xE;
    }
    if (message) {
        func_80049CB4(0x1129,10);
        func_80049CB4(0x128,0x2C);
        func_800498E4(message);
        func_80049CB4(0x1129,10);
        D_80148090=0;
    }
    self->previous_EC=self->hunger_88;
    return okay;
}
