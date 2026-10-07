#include "common.h"
typedef unsigned char u8;
typedef struct { s32 kind; s32 field_4; void *payload; s32 field_C; s32 field_10; } Message;
typedef struct { u8 field_0[0x20]; short field_20,field_22; s32 (*field_24)(void *); } ChildVTable;
typedef struct { s32 field_0; ChildVTable *field_4; } Child;
typedef struct { u8 field_0[0x58]; short field_58,field_5A; s32 (*field_5C)(void *,Message *); } VTable;
typedef struct { u8 field_0[0x24]; VTable *field_24; u8 field_28[0x30]; void *field_58; u8 field_5C[0x30]; Child *field_8C; u8 field_90[0xA]; unsigned short field_9A; } Object;
extern s32 func_80107EC0(Object *),func_800F10F8(Object *,void *,s32,s32,s32);
extern void *func_800F1750(Object *);
extern s32 func_800A4520(void *self,void *target);
extern s32 func_800E20CC(void *obj);
static inline s32 test_flag(Object *arg) { return arg->field_9A&0x40; }
static inline Message *init_message(Message *message,void *value) { message->kind=0xE; message->payload=value; message->field_10=0; return message; }
s32 func_8010841C(Object *arg,void *value) {
    if(!arg->field_8C->field_4->field_24((char *)arg->field_8C+arg->field_8C->field_4->field_20)) {
        if(func_80107EC0(arg)) return 1;
        return !func_800A4520(arg,value) || func_800E20CC(arg);
    } else {
        Message message;
        Message *event;
        void *linked=arg->field_58;
        s32 active=test_flag(arg)!=0;
        switch(func_800F10F8(arg,linked,0,active,0)) { case 1: return 0; case 2: return 1; }
        event=init_message(&message,func_800F1750(arg));
        arg->field_24->field_5C((char *)arg+arg->field_24->field_58,event);
        return 1;
    }
}
