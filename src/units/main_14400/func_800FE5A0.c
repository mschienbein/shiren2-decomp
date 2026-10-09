#include "common.h"
typedef unsigned char u8;
typedef struct { u32 field_0:22; u32 special:1; u32 field_23:9; } Flags;
typedef struct { u8 field_0[0x98]; short field_98,field_9A; void *(*field_9C)(void *); } VTable;
typedef struct { u8 field_0[0x1E]; u8 field_1E,field_1F; Flags field_20; VTable *field_24; } Object;
typedef struct { u8 field_0[0x10]; short field_10,field_12; s32 (*field_14)(void *); u8 field_18[0x20]; short field_38,field_3A; s32 (*field_3C)(void *,s32 *); } ItemVTable;
typedef struct { u8 field_0,field_1,field_2,field_3; s32 field_4; ItemVTable *field_8; } Item;
extern s32 func_800F069C(Object *),func_80049CB4(s32,...);
extern char *func_800A3B20(void *),*func_800AE674(void *);
extern s32 func_800E0F40(Object *);
extern void func_80049AE8(s32,...),func_800497F0(s32,...),func_800E3678(Object *,Object *);
/* Iterator constructor: returns its receiver; the 16-byte iterator storage stays opaque. */
extern void *func_800CEAF0(void *iterator,void *collection,s32 mode);
extern s32 func_800CEBA0(s32 *);
extern Item *func_800CEC68(s32 *);
static inline s32 is_special(const Flags *flags) { return flags->special; }
s32 func_800FE5A0(Object *arg,Object *other) {
    s32 message[8];
    s32 iterator[4];
    Flags flags;
    s32 rejected=0;
    if(!other || !(other->field_1E&0xC) || !other->field_24->field_9C((char *)other+other->field_24->field_98) || !func_800F069C(arg)) rejected=1;
    if(!rejected) {
        flags=other->field_20;
        if(is_special(&flags)) {
            s32 text;
            func_80049CB4(0x1131); func_80049CB4(6);
            text=func_80049CB4(0x4C,arg);
            func_80049CB4(7);
            func_80049AE8(0x131,text,func_800A3B20(arg));
            func_800E3678(other,arg);
            func_800497F0(0x224,text);
            return 1;
        }
        { s32 text=-2;
        s32 applied=0;
        {
            void *source=other->field_24->field_9C((char *)other+other->field_24->field_98);
            message[0]=0x19;
            message[6]=1;
            func_800CEAF0(iterator,source,0);
        }
        for(;;) {
            Item *item;
            s32 eligible;
            if(!func_800CEBA0(iterator)) break;
            item=func_800CEC68(iterator);
            eligible=0;
            if(!item->field_8->field_14((char *)item+item->field_8->field_10)
                && ((u8)func_800E0F40(arg)==3 || (item->field_2&4))
                && ((u8)func_800E0F40(arg)!=1 || item->field_0==4)) {
                if(item->field_8->field_3C((char *)item+item->field_8->field_38,message)) eligible=1;
            }
            if(eligible) {
                if(!applied) {
                    func_80049CB4(0x1131); func_80049CB4(6);
                    text=func_80049CB4(0x4C,arg);
                    func_80049CB4(7); func_80049CB4(6);
                    func_80049CB4(0x23,other,0,0x8000);
                    func_80049CB4(7);
                    func_80049AE8(0x131,text,func_800A3B20(arg));
                    func_800E3678(other,arg);
                    applied=1;
                    func_80049CB4(0x128,0x2B);
                }
                func_80049AE8(0x132,text,func_800AE674(item));
            }
        }
        return applied;
        }
    }
    return 0;
}
