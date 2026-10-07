#include "common.h"
typedef struct { s32 x,y; } Coord;
typedef struct { char pad[8]; short field_8; void (*field_c)(void*,s32); } Methods;
typedef struct { char pad[0x24]; Methods *field_24; } Obj;
typedef struct { char pad[0x400]; signed char field_400; } Owner;
extern unsigned char D_80142F1B,D_80142EF5;
extern Coord *D_801476B8;
extern Obj *func_800AA63C(void);
extern s32 func_800A5E24(Obj*,Coord*,s32,s32),func_800A5B98(void *obj,void *out_position);
extern void func_800A58FC(Obj*,Coord*),func_800F0130(Obj*);
static inline Coord *copy_position(Coord *p,const Coord *q) { p->x=q->x; p->y=q->y; return p; }
static inline void destroy(Obj *item) { if(item) item->field_24->field_c((char*)item+item->field_24->field_8,3); }
void func_800BFBFC(Owner *owner) {
    if(!((D_80142F1B>>2)&1)) {
        s32 i=D_80142EF5;
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
