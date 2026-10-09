#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef struct { s32 x,y; } Coord;
typedef struct { char pad[8]; short field_8; void (*field_c)(void*,s32); } Methods;
typedef struct { char pad[0x24]; Methods *field_24; } Obj;
typedef struct { char pad[0x400]; signed char field_400; } Owner;
/* Whole 0x26-byte floor record at D_80142EF0 (0x80142EF0..0x80142F15): func_800ABBA0
 * fills it with one func_8006AC30 copy (stride 0x26, count 1); bytes are read with lbu
 * at +0x00..+0x25 and the halfword at +0xE with lhu (func_800AB044). */
typedef struct {
    unsigned char field_00, field_01, field_02, field_03, field_04, field_05, field_06, field_07;
    unsigned char field_08, field_09, field_0A, field_0B, field_0C, field_0D;
    unsigned short field_0E;
    unsigned char field_10, field_11, field_12, field_13, field_14, field_15, field_16, field_17;
    unsigned char field_18, field_19, field_1A, field_1B, field_1C, field_1D, field_1E, field_1F;
    unsigned char field_20, field_21, field_22, field_23, field_24, field_25;
} FloorRecord;
extern FloorRecord D_80142EF0;
extern Coord *D_801476B8;
extern Obj *func_800AA63C(void);
extern s32 func_800A5E24(Obj*,Coord*,s32,s32),func_800A5B98(void *obj,void *out_position);
extern void func_800A58FC(Obj*,Coord*),func_800F0130(Obj*);
static inline Coord *copy_position(Coord *p,const Coord *q) { p->x=q->x; p->y=q->y; return p; }
static inline void destroy(Obj *item) { if(item) item->field_24->field_c((char*)item+item->field_24->field_8,3); }
void func_800BFBFC(Owner *owner) {
    if(!((D_80142F18.flags>>2)&1)) {
        s32 i=D_80142EF0.field_05;
        for(;;) {
            Obj *item;
            i--;
            if(i==-1) break;
            item=func_800AA63C();
            if(item) {
                Coord position;
                s32 ok;
                Coord *at=copy_position(&position,D_801476B8);
                if(owner->field_400==11) ok=func_800A5E24(item,at,0,0);
                else ok=func_800A5B98(item,at);
                if(ok) { func_800A58FC(item,&position); func_800F0130(item); }
                else destroy(item);
            }
        }
    }
}
