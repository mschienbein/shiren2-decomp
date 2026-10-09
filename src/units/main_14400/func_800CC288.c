#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionRecord D_80142F18;
extern SelectionSave D_80142F24;
typedef struct { char s[4]; } Str4;
typedef struct { char s[0x50]; } Str80;
typedef struct { unsigned char id; unsigned char flags; } Slot;
typedef struct {
    unsigned char x0;
    unsigned char x1;
    unsigned char x2;
    unsigned char x3;
    Str4 x4;
    unsigned char x8;
    unsigned char x9;
    unsigned char xA;
    char padB;
    u32 xC;
    s32 x10;
    unsigned char x14;
    unsigned char x15;
    unsigned char x16;
    Str80 name;
    unsigned char count;
    short x68;
    char pad6A[2];
    s32 x6C;
    char x70[0x13];
    char x83[0x13];
    Slot slots[2];
} Rec;
typedef struct { char pad[2]; Str4 x2; char pad6[0x12]; s32 x18; } Info;
typedef struct EntVT { char pad[0x10]; short off; s32 (*fn)(void *); } EntVT;
typedef struct { char pad; unsigned char x1; char pad2[6]; EntVT *vt; unsigned char xC; } Ent;
typedef struct PlVT {
    char pad[0x68];
    short off68;
    s32 (*fn6C)(void *);
    short off70;
    u32 (*fn74)(void *);
    char pad78[0x20];
    short off98;
    void *(*fn9C)(void *);
} PlVT;
typedef struct {
    char pad[0x24];
    PlVT *vt;
    char pad28[0x50];
    s32 x78;
    char pad7C[8];
    s32 x84;
    char pad88[8];
    s32 x90;
    char pad94[0x75];
    unsigned char x109;
    unsigned char x10A;
    unsigned char x10B;
} Player;
extern Player *D_801476B8;
extern unsigned char D_80139610;
s32 func_800E0F40(void *);
Info *func_800C9E10(void);
s32 func_800CF3F0(void *inv);
s32 func_800A9A44(unsigned char, unsigned char);
s32 func_800A99A8(void);
unsigned short func_800E08F0(void *);
s32 func_800EB3A4(void *);
Str80 *func_8004935C(s32 v);
s32 func_8005DFE8(char *s);
char *func_80048480(unsigned short);
u32 func_80032D70(const char *);
void *func_800E8A68(void *, unsigned char);
void func_800CB564(char *dst, char *src);
unsigned char func_800E8B10(void *, void *);
/* Byte reads of the selection save, integrated at each call site. */
static inline unsigned char selIndex(SelectionSave *s) { return s->index; }
static inline unsigned char selCount(SelectionSave *s) { return s->count; }
static inline unsigned char selBonus(SelectionSave *s) { return s->field_06; }
void func_800CC288(Rec *r) {
    Player *pl = D_801476B8;
    void *inv = pl->vt->fn9C((char *)pl + pl->vt->off98);
    Info *info;
    s32 n;
    char *p;
    s32 i;
    char sep[2];
    Ent *ents[2];
    Slot *slot;
    s32 k;
    r->x0 = selIndex(&D_80142F24);
    r->x1 = selCount(&D_80142F24);
    r->x2 = D_80142F18.kind;
    r->x3 = func_800E0F40(pl);
    r->x4 = func_800C9E10()->x2;
    r->x8 = pl->x109;
    r->x9 = pl->x10A;
    r->xA = pl->x10B;
    n = 1;
    r->xC = pl->x84;
    if ((unsigned char)(r->x8 - 45) < 12) {
        r->xC += func_800CF3F0(inv);
        n += (unsigned char)func_800A9A44(r->x0, r->x1);
    } else if (r->x2 != 0) {
        n = r->x2;
        if (func_800A99A8()) n += selBonus(&D_80142F24);
    }
    r->xC += (n - 1) * 5000;
    if (r->xC > 999999999) r->xC = 999999999;
    info = func_800C9E10();
    r->x10 = info != 0 ? info->x18 : 0;
    r->x10 = -pl->x90 + r->x10;
    r->x68 = func_800E08F0(pl);
    r->x14 = func_800EB3A4(pl);
    r->x15 = pl->vt->fn6C((char *)pl + pl->vt->off68);
    r->x16 = pl->vt->fn74((char *)pl + pl->vt->off70);
    r->x6C = pl->x78;
    r->name = *func_8004935C(0);
    p = r->name.s;
    i = 0;
    r->count = D_80139610;
    for (;;) {
        char *q;
        if (i >= r->count) break;
        if (func_8005DFE8(p) > 0xF0) {
            char *s = func_80048480(0x53C);
            sep[0] = s[0];
            sep[1] = s[1];
            q = p;
            for (;;) {
                unsigned char c = *q;
                s32 step;
                if (c == 0) break;
                step = (c & 0xF0) == 0xF0 ? 2 : 1;
                if (c == sep[0] && q[1] == sep[1]) {
                    char *dst;
                    q[0] = 0;
                    dst = q + 1;
                    q += 2;
                    while (q < r->name.s + sizeof(r->name)) *dst++ = *q++;
                    *dst = 0;
                    r->count++;
                    break;
                }
                q += step;
            }
        }
        p += func_80032D70(p) + 1;
        i++;
    }
    func_800CB564(r->x70, func_800E8A68(pl, 3));
    func_800CB564(r->x83, func_800E8A68(pl, 4));
    func_800E8B10(pl, ents);
    slot = r->slots;
    for (k = 0;; slot++, k++) {
        Ent *e;
        if (k >= 2) break;
        e = ents[k];
        if (e != 0) {
            slot->id = e->x1;
            if (e->xC) slot->flags |= 1;
            else slot->flags &= ~1;
            if (e->vt->fn((char *)e + e->vt->off)) slot->flags |= 2;
            else slot->flags &= ~2;
        } else {
            slot->id = 0;
        }
    }
}
