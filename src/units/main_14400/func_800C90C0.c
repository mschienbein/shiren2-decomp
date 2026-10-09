#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef struct {
    u8 field_00[8]; short field_08, field_0A; void (*field_0C)(void *, s32);
    short field_10, field_12; s32 (*field_14)(void *);
} VTable;
typedef struct Object {
    u8 field_00[0x1E]; u8 field_1E; u8 field_1F[5]; VTable *field_24;
    u8 field_28[0x30]; struct Object *field_58;
} Object;
typedef struct { s32 x, y; } Pos;
extern Pos D_80147664;
extern s32 D_80147678;
/* Game-state flag word (.data, 0x8014767C); this file owns it. */
u16 D_8014767C = 0;
extern u8 D_801480CE, D_801476BD, D_80143392;
extern s32 func_800A8FC8(s32 *,s32),func_800A99D0(void),func_80046240(void),func_800A6348(Object *,void *);
extern s32 func_800A8F6C(s32 *iterator);
extern Object *func_800A910C(s32 *),*func_800AA86C(void),*func_800AA63C(void);
extern void *func_800C5F60(void);
extern void func_800C942C(void),func_800F0130(Object *);
extern s32 func_800D74AC(void);
extern void func_800E2298(void *obj);
extern s32 func_80049CB4(s32 id, ...);
extern s32 func_800A251C(Pos *, Pos *);
extern void *func_800B1F90(Pos *);
extern void func_800B1CEC(s32, Pos *);
extern void func_800B2DC4(Pos *);
extern void func_800C95C8(void *);
void func_800C90C0(void) {
    s32 cursor=0;
    while(func_800A8FC8(&cursor,0x7C)) {
        Object *obj=func_800A910C(&cursor);
        if(obj->field_58 && obj->field_58->field_24->field_14((char *)obj->field_58+obj->field_58->field_24->field_10)) obj->field_58=0;
    }
    func_800C942C();
    if((D_8014767C>>6)&1) return;
    if(D_801480CE) { func_800D74AC(); return; }
    { s32 ready=!func_800A99D0() && !func_80046240();
    if(ready) {
        void *value=func_800C5F60();
        switch(D_8014767C&0xC) {
        case 4: {
            s32 i;
            for(i=9;i!=-1;--i) { Object *obj=func_800AA86C(); if(obj) func_800A6348(obj,value); }
            D_8014767C=(D_8014767C&0xFFF3)|8;
            break;
        }
        case 8: { Object *obj=func_800AA86C(); if(obj) func_800A6348(obj,value); break; }
        default:
            if(!((D_80142F18.flags>>6)&1)) {
                if(--D_801476BD==0) {
                    Object *obj;
                    D_801476BD=0x32;
                    obj=func_800AA63C();
                    if(obj && func_800A6348(obj,value)) func_800F0130(obj);
                }
            }
            break;
        }
    }
    }
}
/* Whole-object setters preserve the original independent member stores. */
static inline void clear_x(Pos *pos) { pos->x = 0; }
static inline void clear_y(Pos *pos) { pos->y = 0; }
void func_800C92BC(void *obj) {
    func_800E2298(obj);
    func_80049CB4(0xDB);
    func_80049CB4(2);
    clear_x(&D_80147664);
    clear_y(&D_80147664);
}
void func_800C92F8(Pos *s) {
    Pos p;
    Pos *pp = &p;
    Pos *last = &D_80147664;
    void *t;
    pp->x = s->x;
    pp->y = s->y;
    if (func_800A251C(pp, last)) return;
    if (D_80143392 == 0) {
        t = func_800B1F90(pp);
        if (t != 0 && t == func_800B1F90(last)) {
            func_800B1CEC(1, pp);
            return;
        }
    }
    func_800E2298(s);
    func_800B2DC4(&D_80147664);
    D_80147664 = p;
}
s32 func_800C93CC(void)
{
    u16 flags = D_8014767C;
    s32 result = 0;

    if ((flags >> 7) & 1) {
        result = (flags >> 8) & 1;
    }
    return result;
}
void func_800C93F4(void) {
    D_8014767C = (D_8014767C & ~0xC) | 4;
    func_80049CB4(0x126, 0x20);
}
/* D_80158C98 slots 0x0C/0x14 target func_800E016C/func_800E24A8. */
void func_800C942C(void) {
    s32 iterator = 0;
    while (func_800A8F6C(&iterator)) {
        Object *object = func_800A910C(&iterator);
        s32 eligible = 0;
        VTable *table = object->field_24;
        if (table->field_14((u8 *)object + table->field_10)) {
            u32 flag = (object->field_1E >> 2) & 1;
            eligible = flag < 1;
        }
        if (eligible && object) {
            table = object->field_24;
            table->field_0C((u8 *)object + table->field_08, 3);
        }
    }
}
s32 func_800C94D8(void) { return D_80147678 == 0; }
void func_800C94E8(void) { D_8014767C |= 0x10; }
void func_800C9500(void) {
    if ((D_8014767C >> 4) & 1) {
        func_80049CB4(0xDB);
        D_8014767C &= ~0x10;
    }
}
static inline s32 flag_is_clear(void) { return ((D_8014767C >> 5) & 1) ^ 1; }
void func_800C9544(void) {
    if (flag_is_clear()) {
        D_8014767C |= 0x20;
        func_800C94E8();
        func_800C95C8(0);
    }
}
void func_800C9588(void) {
    if ((D_8014767C >> 5) & 1) {
        D_8014767C &= ~0x20;
        func_800C94E8();
        func_800C95C8(0);
    }
}
