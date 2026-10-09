#include "common.h"
inline void *operator new(unsigned int, void *p) { return p; }
extern "C" {

typedef struct { short delta; short index; s32 (*fn)(void *); } VEntry;
typedef struct { short delta; short index; void *(*fn)(void *); } OwnerEntry;
typedef struct { s32 unk0; VEntry *vtbl; } Owner;
typedef struct { char pad[0x24]; OwnerEntry *vtbl; } Party;
typedef struct { s32 *unk0; } Member;
typedef struct {
    Owner *owner;       /* 0x00 */
    Member *member;     /* 0x04 */
    unsigned short unk8;/* 0x08 */
    s32 (*filter)(void *); /* 0x0C: item filter, 0 = owner default */
    void *list;         /* 0x10 */
} Ctx;
typedef struct { s32 unk0; unsigned char *info; } Entry;
/* The selection entry is dead before the confirmation result is produced. */
union Scratch { Entry entry; s32 answer; };
typedef struct {
    char pad0[0x4C];
    void *vtbl;         /* 0x4C */
    char pad50[0x18];
    s32 unk68;          /* 0x68 */
    void *unk6C;        /* 0x6C */
    char pad70[0xA0];
} Dialog;               /* 0x110 */
typedef struct { s32 w[4]; } Reply;
extern Party *D_801476B8;
/* Whole session panel, 0x40C bytes (the next panel begins at D_801408EC). */
typedef struct {
    char pad0[0x2CC];
    s32 field_2CC;
    char pad_2D0[0x1C];
    s32 (*filter_2EC)(void *);
    char pad_2F0[0x118];
    unsigned short message_408;
    unsigned short pad_40A;
} Panel;
extern Panel D_801404E0;
extern char D_8014AB48[];
extern char D_8014AB50[];
extern char D_8014AB60[];
extern char D_80151E38[];
extern char D_80151EC8[];
extern char D_80152AE8[];
extern s32 func_800CE46C(void *list, s32 (*cb)(void *));
extern s32 func_800CD278(Member *);
extern s32 func_800AF950(s32 *);
extern void func_80097B90(void *obj, void *owner, void *holder, s32 mode, void *desc, s32 flag);
extern void func_80099E50(void *);
extern s32 func_800957C0(void *object, void *output, s32 modal, void *history, s32 event);
extern s32 func_8009A0EC(void *);
extern s32 func_8009A038(void *);
extern void *func_800D8FB0(u32 size);
extern char *func_800DE730(void *, Member *);
extern Entry func_8009A054(void *, s32);
extern void func_800D05A4(void *, Entry *);
extern Dialog *func_800953C0(Dialog *);
extern char *func_80048480(unsigned short);
extern void func_8009D610(void *object, char *text, void *rect, void *position);
extern void func_8009D6E4(Dialog *, s32);

/* ODD_C: opens the session panel for the owner; passing the panel through the inline
   parameter keeps its address symbolic for the following field stores (no base
   register held across the call), which also shapes register allocation. */
static inline void panel_open(Panel *panel, Owner *owner, s32 mode) { func_80097B90(panel, owner, 0, mode, D_8014AB48, 0); }
static inline void panel_filter(Panel *panel, s32 (*filter)(void *)) { panel->filter_2EC = filter; }
static inline void panel_message(Panel *panel, unsigned short message) { panel->message_408 = message; }

s32 func_800A1308(Ctx *ctx, unsigned short msg) {
    s32 ok;
    s32 flags;
    s32 count;
    s32 total;
    s32 i;
    char *list;
    Owner *owner;
    Owner *leader;

    if (ctx->filter == 0) {
        Owner *self = ctx->owner;
        if (self->vtbl[4].fn((char *)self + self->vtbl[4].delta) == 0) return 0;
    } else {
        ok = func_800CE46C(ctx->owner, ctx->filter) != 1;
        if (ok) return 0;
    }
    if (func_800CD278(ctx->member) == 0) return 1;
    ok = func_800AF950(ctx->member->unk0) != 1;
    if (ok) return 1;
    leader = (Owner *)D_801476B8->vtbl[19].fn((char *)D_801476B8 + D_801476B8->vtbl[19].delta);
    owner = ctx->owner;
    i = 0x1000; /* mode: leader 0x1000, other 0x200 (reuses i's register) */
    if (owner != leader) i = 0x200;
    panel_open(&D_801404E0, owner, i);
    panel_message(&D_801404E0, ctx->unk8);
    D_801404E0.field_2CC = 1;
    panel_filter(&D_801404E0, ctx->filter);
retry:
    {
        Reply reply;
        Scratch scratch;
        func_80099E50(&D_801404E0);
        ok = func_800957C0(&D_801404E0, &reply, 1, 0, 0) != 1;
        if (ok) return 2;
        flags = func_8009A0EC(&D_801404E0);
        if (flags & 1) return 5;
        count = func_8009A038(&D_801404E0);
        total = 0;
        list = func_800DE730(func_800D8FB0(0xCC), ctx->member);
        i = 0;
        for (;;) {
            s32 more = i < count;
            if (!more) break;
            {
                new (&scratch.entry) Entry(func_8009A054(&D_801404E0, i));
                total += scratch.entry.info[4];
                func_800D05A4(list + 0xB0, &scratch.entry);
                i++;
            }
        }
        if (func_800CD278(ctx->member) < total) return 3;
        ctx->list = list;
        if (flags & 2) {
            if (msg != 0) {
                Dialog dialog;
                Dialog *d = &dialog;
                func_800953C0(d);
                d->vtbl = D_80152AE8;
                dialog.unk68 = -1;
                dialog.unk6C = D_80151EC8;
                func_8009D610(d, func_80048480(msg), D_8014AB50, D_8014AB60);
                func_8009D6E4(d, 0);
                ok = func_800957C0(d, &scratch.answer, 1, 0, 0) != 1;
                if (ok) scratch.answer = 0;
                if (scratch.answer) {
                    d->vtbl = D_80151E38; /* destroy dialog */
                    return 7;
                }
                d->vtbl = D_80151E38; /* destroy dialog */
                goto retry;
            }
            return 6;
        }
    }
    return 7;
}

}
