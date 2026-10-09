#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef struct { s32 x,y; } Position;
typedef struct { u8 field_0[0x10]; short field_10,field_12; s32 (*field_14)(void *); } EntityVTable;
typedef struct { u8 field_0[9]; u8 field_9; u8 field_A[0x12]; u16 field_1C; u8 field_1E; u8 field_1F[5]; EntityVTable *field_24; } Entity;
typedef struct { u8 value; } Kind;
typedef struct { s32 field_0; u8 field_4[8]; Kind field_C; u8 field_D[3]; Position field_10; u8 field_18[8]; } Message;
/* Item table at item+8, slot +0x38/+0x3C: message handler s32 (void *receiver, void *event)
 * (decided message-handler contract); the result is not needed here. */
typedef struct { u8 field_0[0x38]; short field_38,field_3A; s32 (*field_3C)(void *receiver,void *event); } ItemVTable;
typedef struct { u8 field_0,field_1,field_2,field_3; s32 field_4; ItemVTable *field_8; } Item;
typedef struct { u8 field_0[0xC]; short field_C; u8 field_E[0xA]; } Damage;
typedef struct { void *field_0; s32 field_4; s8 field_8; } Request;
typedef struct { s8 x; s8 y; } Offset;
extern const Offset D_801428F0[9];
extern s8 D_80142910;
extern Request D_80142904;
extern s32 func_800C94D8(void),func_80049CB4(s32,...),func_800A58B8(Entity *);
extern u32 func_800B1C6C(void *pos);
extern u16 func_800E08B0(Entity *);
extern void func_800B4788(Position *),func_800B4E7C(Position *),func_80136910(Damage *,void *,u32,u32,u32),func_800A7ADC(Entity *,Damage *),func_800A7B68(Entity *,Damage *);
extern Item *func_800B4D80(Position *);
extern Entity *func_800B4928(Position *);
static inline s32 state_flags(Position *origin) { return func_800B1C6C(origin)&0x100; }
static inline s32 category(Entity *entity) { return entity->field_9&0xF; }
static inline Position *offset_position(Position *out,Position *origin,s32 dx,s32 dy) {
    s32 x=origin->x+dx;
    s32 y=origin->y+dy;
    out->x=x;
    out->y=y;
    return out;
}
static inline Kind *set_kind(Kind *kind,u8 value) { kind->value=value; return kind; }
static inline u8 read_kind(Kind *kind) { return kind->value; }
static inline s32 item_type(Item *item) { return item->field_1; }
void func_800A00C4(Position *origin,u8 percentage,void *attacker,s32 kind) {
    u8 flags[9];
    Position position;
    Damage damage;
    Message message;
    Kind kindTag;
    s32 failed=func_800C94D8()!=1;
    if(!failed && D_80142910<0x14) {
        s32 clear;
        s32 i;
        s32 firstHit,priority;
        D_80142910=(u8)D_80142910+1;
        D_80142904.field_0=attacker;
        D_80142904.field_4=kind;
        D_80142904.field_8=percentage;
        func_80049CB4(0x131);
        clear=0;
        if(!(func_800B1C6C(origin)&0x2000)) clear=!state_flags(origin);
        if(clear) { func_80049CB4(6); func_800B4788(origin); func_80049CB4(7); }
        i=9;
        for(;;) {
            Item *item;
            Entity *entity;
            s32 eligible;
            u8 *flag;
            Position *p;
            if(--i==-1) break;
            p=offset_position(&position,origin,D_801428F0[i].x,D_801428F0[i].y);
            flag=&flags[i];
            *flag=0;
            item=func_800B4D80(p);
            if(item && item->field_3==2) {
                switch(item->field_0) {
                case 15: break;
                case 16: {
                    s32 mark=0;
                    if(item_type(item)==0xDD || item_type(item)==0xDE) mark=1;
                    if(mark) *flag|=0x10;
                    break;
                }
                case 19: if(item->field_1!=0xF2) break;
                default:
                    func_80049CB4(6); func_80049CB4(0x107,&position); func_80049CB4(7);
                    func_800B4E7C(&position); func_80049CB4(0xD7,&position);
                    break;
                }
            }
            entity=func_800B4928(&position);
            eligible=0;
            if(entity && !entity->field_24->field_14((char *)entity+entity->field_24->field_10)
                && !(entity->field_1C&1) && category(entity)<3) eligible=func_800A58B8(entity)>=2;
            if(eligible) { if(entity->field_1E&0xC) *flag|=3; else *flag|=1; }
        }
        func_80136910(&damage,attacker,0,kind,0x408);
        firstHit=1;
        priority=4;
        for(;;) {
            s32 finished;
            if(--priority==0) break;
            i=9;
            for(;;) {
                Entity *entity;
                u8 *flag;
                Position *p;
                if(--i==-1) break;
                p=offset_position(&position,origin,D_801428F0[i].x,D_801428F0[i].y);
                flag=&flags[i];
                entity=func_800B4928(p);
                if(entity && !entity->field_24->field_14((char *)entity+entity->field_24->field_10) && priority==(*flag&3)) {
                    if(entity->field_1E&0xC) {
                        s32 amount=func_800E08B0(entity);
                        damage.field_C=(amount*(u8)percentage)/100;
                        if(!damage.field_C) damage.field_C=1;
                        if((*flag&3)==3 && damage.field_C>=amount) *flag=(*flag&0xFC)|2;
                        else {
                            if(firstHit) entity->field_1C|=0x100;
                            func_800A7ADC(entity,&damage);
                            if(firstHit) entity->field_1C&=0xFEFF;
                            firstHit=0;
                        }
                    } else func_800A7B68(entity,&damage);
                    { s32 finished=func_800C94D8()!=1; if(finished) break; }
                }
            }
            finished=func_800C94D8()!=1;
            if(finished) break;
        }
        if(func_800C94D8()) {
            i=9;
            for(;;) {
                Item *item;
                if(--i==-1) break;
                item=func_800B4D80(offset_position(&position,origin,D_801428F0[i].x,D_801428F0[i].y));
                if((flags[i]&0x10) && item) {
                    Kind *selected=set_kind(&kindTag,2);
                    message.field_0=0x15;
                    message.field_10=position;
                    message.field_C.value=read_kind(selected);
                    item->field_8->field_3C((char *)item+item->field_8->field_38,&message);
                }
            }
        }
        D_80142910=(u8)D_80142910-1;
    }
}
