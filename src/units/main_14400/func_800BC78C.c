#include "common.h"
#include "map_rect.h"

/* Complete 0x14-byte room: geometry followed by four unsigned side counts. */
typedef struct { MapRect bounds; unsigned char side_count[4]; } Entry;
/* D1DBC writes these fields in each 0x18-byte owner record. The target ABI
 * supplies alignment before room_04 and field_0C; no geometry is byte storage. */
typedef struct {
    signed char owner_00;
    Entry *room_04;
    unsigned char selection_08;
    s32 field_0C;
    s32 field_10;
    s32 field_14;
} Slot;
/* Borrowed generator view, never allocated here. Its uninterpreted ranges
 * cover foreign subobjects; this is not their allocation or lifetime model. */
typedef struct { char pad0[0x3DC]; s32 count; char pad3E0[0x57C]; s32 available[16]; } Object;
/* Foreign initialized backing: these declarations allocate no room/owner/RNG
 * storage and do not establish the foreign C++ objects' lifetimes. */
extern char D_80147620[];
extern Entry D_801431F0[16];
extern Slot D_80143330[2];
extern unsigned char D_80156ABF;
extern unsigned char func_800C57CC(void *,s32);
extern s32 func_800B68B0(Entry *),func_800A2854(MapPos *,MapPos *,MapPos *),func_800C587C(void *,unsigned char);
extern MapPos *func_800B6A98(MapPos *out,Entry *room,s32 index);
extern MapPos *func_800A256C(MapPos *,MapPos *,MapPos *),*func_800A2544(MapPos *,MapPos *,MapPos *);
extern void func_800D1DBC(Slot *,signed char,Entry *),func_800BC69C(Object *,MapRect *,s32);
static inline MapPos *initPosition(MapPos *p,s32 x,s32 y) { p->x=x; p->y=y; return p; }
s32 func_800BC78C(Object *obj,unsigned char id) {
    MapPos origin,point,low,deltaLow,high,deltaHigh;
    unsigned char slot=id;
    Entry *entry=0;
    s32 remaining=100;
    for(;;) {
        s32 left=--remaining;
        s32 index,type;
        if(left==-1) break;
        index=func_800C57CC(D_80147620,(unsigned char)(obj->count-1));
        if(!obj->available[index]) continue;
        entry=&D_801431F0[index];
        type=func_800B68B0(entry);
        if(type>=3) continue;
        if(type==2) {
            MapPos *lower,*upper;
            func_800B6A98(&origin,entry,0);
            func_800B6A98(&point,entry,1);
            lower=&low;
            func_800A256C(lower,&origin,initPosition(&deltaLow,1,1));
            upper=&high;
            func_800A2544(upper,&origin,initPosition(&deltaHigh,1,1));
            if(func_800A2854(&point,lower,upper)) continue;
        }
        obj->available[index]=0;
        break;
    }
    if(remaining<0) return 0;
    func_800D1DBC(&D_80143330[slot],(signed char)slot,entry);
    func_800BC69C(obj,&entry->bounds,func_800C587C(D_80147620,D_80156ABF));
    return 1;
}
