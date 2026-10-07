#include "common.h"

typedef unsigned char u8;
typedef struct { s32 x,y; } Position;
typedef struct { char pad0[0x20]; short offset20; short pad22; s32 (*method24)(void *); char pad28[0x50]; short offset78; short pad7A; s32 (*method7C)(void *,Position *); short offset80; short pad82; void (*method84)(void *,Position *); } VTable;
typedef struct { s32 field0; VTable *vtable; } Child;
typedef struct { char pad0[0x20]; s32 stride; char pad24[0x10]; Position position; char pad3C[0x10]; VTable *vtable; char pad50[0x27C]; s32 enabled; char pad2D0[0x1C]; s32 (*callback)(void *); Child *child; char pad2F4[8]; s32 mode; char pad300[0xD8]; s32 maximum; } Object;
extern void *func_800980F0(void *c,s32 idx);
extern s32 func_80099F98(Object *),func_80098E34(Object *,s32),func_80098F54(Object *,s32);
extern void func_80045A24(s32),func_80099E50(Object *),func_80098718(Object *);
extern u8 *func_8006A810(void *dst,s32 value,s32 count);
extern s32 func_800D8FF0(void *obj);
extern void *func_800D8FB0(u32 size),*func_800D9DD0(void *);
static inline void updatePosition(Object *obj,s32 key) {
    s32 row=func_80098F54(obj,key)/10;
    s32 column=func_80098F54(obj,key)%10;
    if(row!=obj->position.y || column!=obj->position.x) {
        Position position;
        func_8006A810(&position,0,8);
        position.x=column;
        position.y=row;
        obj->vtable->method84((char *)obj+obj->vtable->offset80,&position);
    }
}
s32 func_80098B90(Object *obj,s32 event) {
    s32 failed;
    if(event==0x36) {
        failed=obj->enabled!=1;
        if(failed) return -1;
        if(obj->callback) {
            void *selected=func_800980F0(obj,obj->vtable->method7C((char *)obj+obj->vtable->offset78,&obj->position));
            if(!selected) return -1;
            failed=obj->callback(selected)!=1;
            if(failed) return -1;
        }
        if(func_80099F98(obj)) func_80045A24(0);
        else func_80045A24(1);
        return -1;
    }
    if(event==0x35 && (obj->mode==1 || obj->mode==0x1000 || obj->mode==0x800) && obj->position.y<obj->maximum) {
        Child *child;
        s32 old,key;
        func_80099E50(obj);
        child=obj->child;
        old=child->vtable->method24((char *)child+child->vtable->offset20);
        func_800D8FF0(func_800D9DD0(func_800D8FB0(8)));
        func_80045A24(4);
        func_80098718(obj);
        child=obj->child;
        if(child->vtable->method24((char *)child+child->vtable->offset20)!=old) key=func_80098E34(obj,0);
        else key=func_80098E34(obj,obj->position.x+obj->stride*obj->position.y);
        updatePosition(obj,key);
        obj->vtable->method24((char *)obj+obj->vtable->offset20);
    }
    return -1;
}
