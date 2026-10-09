#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

typedef struct {
    u8 kind_00;
    u8 id_01;
    u8 flags_02;
    u8 pad_03[0xC];
    s8 count_0F;
    u8 pad_10[0x18];
    u8 field_28;
    u8 pad_29[0x71];
    u16 flags_9A;
    u8 pad_9C;
    u8 level_9D;
} Ent;
typedef struct { s16 delta; s16 index; void (*func)(void *self, s32 flags); } DestroySlot;
typedef struct { u8 pad_00[8]; DestroySlot destroy; } ObjVtable;
typedef struct { u8 pad_00[8]; ObjVtable *vtable; } Obj;
typedef struct { s32 index; void *owner; s32 reverse; void *current; } Iter;
/* Whole 0x18-byte derived item collection at Floor+0xCC: the 0x10-byte base list
 * (see func_800CE6A0) plus owner and text id (constructed by func_800CEC90). */
typedef struct {
    void *pool;
    void *vtable;
    u8 *buffer;
    u8 capacity;
    u8 limit;
    u8 count;
    u8 pad_0F;
    void *owner_10;
    u16 text_14;
    u8 pad_16[2];
} ItemList;
typedef struct {
    u8 pad_00[0x84];
    s32 field_84;
    u8 pad_88[0xC];
    u8 flags_94;
    u8 pad_95[0x37];
    ItemList list_CC;
} Floor;

extern u32 D_8013960C;
extern u8 D_80147620[];
extern void func_800ECA18(Floor *object);
extern void func_800EDFF0(Floor *self, s32 notify);
extern void func_800E9DA8(Floor *actor, s32 kind);
extern void func_800EB330(Floor *self, u8 value);
extern void func_800EB35C(Floor *obj, u8 seconds);
extern Iter *func_800CEB20(Iter *s, void *a);
extern void func_800CEB54(Iter *s);
extern s32 func_800CEBA0(Iter *it);
extern Ent *func_800CEC68(Iter *iterator);
extern void func_801224FC(void);
extern void func_8012256C(void);
extern s32 func_800AA8D0(void);
extern s32 func_801225B0(void);
extern void *func_800E8A68(Floor *obj, u8 arg1);
extern s32 func_8010BEC4(Ent *s, u8 c);
extern s32 func_8010B9F4(Ent *s);
extern s32 func_800C5844(void *rng, u8 base, u8 top);
extern s32 func_8010BA90(Ent *e, s16 v);
extern u8 func_800C57A0(void *object);
extern s32 func_8010BE60(Ent *list, u8 index);
extern s32 func_800CD2BC(void *object, void *element);
extern void func_800D52D0(void *arg, s32 index);
extern void *func_801142D0(Ent *obj, u32 index);
extern void *func_80128404(Obj *spawn);
extern s32 func_800A6184(void *self, Floor *pos);
extern s32 func_800A8FC8(s32 *index, s32 mask);
extern void *func_800A910C(s32 *it);
extern void *func_800AC5B4(s32 size, s32 alternate);
extern void *func_80128280(void *arg);
extern s32 func_800AC670(void *key);
extern void func_80128388(void *self, Ent *source);
extern s32 func_8010BD3C(Ent *item, s32 code);
extern void func_800CD3D0(void *list, u32 index);
extern void func_800AE518(Ent *self, void *arg, s32 x, s32 y);
extern s32 func_80049CB4(s32 id, ...);

static inline u8 kind_of(Ent *e) { return e->kind_00; }
static inline u8 has_status(Ent *e, u8 c) { return func_8010BEC4(e, c); }

void func_800ECF38(Floor *f, s32 mode) {
    Iter it;
    ItemList *list;
    s32 i;

    f->flags_94 &= ~3;
    func_800ECA18(f);
    func_800EDFF0(f, 1);
    func_800E9DA8(f, mode);
    func_800EB330(f, 100);
    func_800EB35C(f, 100);
    list = &f->list_CC;
    D_8013960C <<= 1;
    func_800CEB20(&it, list);
    if (mode == 2) {
        func_801224FC();
        func_8012256C();
        if (func_800AA8D0()) {
            s32 j;
            i = 0;
            if (func_801225B0()) {
                for (;;) {
                    Ent *e;
                    s32 k;
                    if (i >= 2) break;
                    e = (i == 0) ? func_800E8A68(f, 3) : func_800E8A68(f, 4);
                    if (e != 0) {
                        if (has_status(e, 0x21)) {
                            i++;
                            continue;
                        }
                        if ((s16)func_8010B9F4(e) > 0) {
                            s16 hp = func_8010B9F4(e);
                            func_8010BA90(e, -(hp * (u8)func_800C5844(D_80147620, 10, 50) / 100));
                        }
                        k = e->count_0F;
                        while (k-- > 0) {
                            if (func_800C57A0(D_80147620) & 1) func_8010BE60(e, k);
                        }
                    }
                    i++;
                }
                i = 0;
            }
            j = i;
            for (;;) {
                Ent *e;
                if (j >= 2) break;
                e = (j == 0) ? func_800E8A68(f, 3) : func_800E8A68(f, 4);
                if (e == 0) {
                    j++;
                    continue;
                }
                if (has_status(e, 0x21)) {
                    j++;
                    continue;
                }
                func_800CD2BC(list, e);
                func_800D52D0(e, i);
                i++;
                j++;
            }
        }
        {
            Ent *e;
            Ent *best;
            s32 bestLevel;
            s32 index;
            func_800CEB54(&it);
            while (func_800CEBA0(&it)) {
                Obj *o;
                void *r;
                e = func_800CEC68(&it);
                if (e->id_01 != 0xAC) continue;
                o = func_801142D0(e, 0);
                if (o == 0) continue;
                r = func_80128404(o);
                o->vtable->destroy.func((u8 *)o + o->vtable->destroy.delta, 3);
                if (r) func_800A6184(r, f);
            }
            best = 0;
            bestLevel = -1;
            index = 0;
            while (func_800A8FC8(&index, 0x10)) {
                s32 ok;
                u16 fl;
                e = func_800A910C(&index);
                fl = e->flags_9A;
                ok = 0;
                if (fl & 0x40) {
                    if (!(fl & 0x100)) {
                        s32 t = fl & 0x200;
                        ok = t == 0;
                    }
                }
                if (!ok) continue;
                if (bestLevel < e->level_9D) {
                    best = e;
                    bestLevel = e->level_9D;
                }
            }
            if (best) {
                void *g = func_80128280(func_800AC5B4(0x14, 0));
                s32 failed = func_800AC670(g) != 1;
                if (failed) {
                    func_80128388(g, best);
                    func_800D52D0(g, 2);
                }
            }
        }
    }
    i = 0;
    func_800CEB54(&it);
    while (func_800CEBA0(&it)) {
        s32 keep = 1;
        Ent *e = func_800CEC68(&it);
        s32 trap = 0;
        if (kind_of(e) == 3 || kind_of(e) == 4) trap = 1;
        if (mode == 4 || mode == 5) {
            keep = 0;
        } else if (trap && has_status(e, 0x21)) {
            func_8010BD3C(e, 0x21);
            keep = 0;
        } else if (e->flags_02 & 4) {
            if (mode == 3) keep = 0;
        } else if (e->id_01 == 0xB0) {
            keep = 0;
            i = 1;
        }
        if (keep) func_800CD3D0(list, it.index + 1);
    }
    func_800CEB54(&it);
    while (func_800CEBA0(&it)) {
        Ent *e = func_800CEC68(&it);
        if (e->flags_02 & 4) func_800AE518(e, f, 0, 0);
        if (e->kind_00 == 9) e->field_28 = 1;
    }
    if (mode == 2 && i == 0) f->field_84 = 0;
    D_8013960C >>= 1;
    func_80049CB4(0x89, f);
}
