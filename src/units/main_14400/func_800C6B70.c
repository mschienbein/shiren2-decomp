#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
static inline unsigned char selection_flags(const SelectionRecord *record) { return record->flags; }

static inline unsigned char selection_kind(const SelectionRecord *record) { return record->kind; }

static inline unsigned char selection_variant(const SelectionRecord *record) { return record->variant; }


typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef struct {
    u8 pad[0x20];
    s16 delta20;
    s16 pad22;
    void (*fn20)(void *);
    s16 delta28;
    s16 pad2A;
    void (*fn28)(void *);
    s16 delta30;
    s16 pad32;
    void (*fn30)(void *);
    s16 delta38;
    s16 pad3A;
    void (*fn38)(void *);
} VTable;
typedef struct {
    u8 pad[8];
    VTable *vtable;
} Obj;
extern s16 D_80147660;
extern u16 D_8014767C;
extern s16 D_801476BE;
extern u8 D_801476BD;
extern s32 D_80147678;
extern u8 D_80140160[];



extern u8 D_80139610;
extern u8 D_8013960A;
/* +0x8C word of the 0x90-byte menu system D_80140160 (func_80093CDC reads/writes +0x8C through
 * the object; former label D_801401EC is this field, not a separate object). */
static inline void menu_system_clear_8C(u8 *system) { *(s32 *)(system + 0x8C) = 0; }
extern s32 D_80140254;
extern u8 D_801476BC;
extern s8 D_801476C2;
extern Obj *D_80142B10;
extern void *D_801476B8;
/* 0x10-byte state: func_800D459C stores room at +0 and words at +4/+8/+C. */
typedef struct { void *room; s32 opaque[3]; } RoomState;
extern RoomState D_80143434;
void func_801F226C(s32);
s32 func_80049CB4(s32, ...);
void func_80094B80(void *, s32);
void func_80094DA0(void *);
void func_80041450(s32);
void func_800C77C8(void);
void func_80055B14(u32, u32);
s32 func_80046240(void);
void func_80046C30(s32);
void func_80048380(void);
void func_8007268C(void);
void func_800C9800(s32);
s32 func_80094B98(void *);
void func_800C96FC(void);
void func_800C9870(void);
void func_80094B3C(void *, s32);
s32 func_800C9810(void);
s32 func_800413E0(void);
void func_800B123C(void);
void func_800B2D50(void);
void func_800B2B50(void);
void func_800B6F58(Obj *);
void func_800B7340(Obj *);
s32 func_800B61EC(void *);
void func_801F2108(void);
void func_800D7798(void);
void func_80122600(void);
void func_800C942C(void);
void func_8004569C(void);
void func_800C7830(void);
void func_800C92BC(void *);
void func_800413FC(void);
void func_80046374(void);
void func_80045E50(void);
void func_80094FA8(void *);
void func_80046B08(s32);
void func_800A9A90(void);

void func_800C6B70(void)
{
    s32 prev;
    Obj *obj;

    D_801476BE = 1;
    D_80147660 = 0x78;
    D_801476BD = 0x32;
    D_8014767C &= 0xFE73;
    func_801F226C(0);
    prev = D_80147678;
    D_80147678 = 0;
    func_80049CB4(0xA);
    D_80139610 = 0;
    D_8013960A = 1;
    menu_system_clear_8C(D_80140160);
    func_80094B80(D_80140160, ((selection_flags(&D_80142F18) >> 2) & 1) ^ 1);
    func_80094DA0(D_80140160);
    func_80041450(1);
    D_80140254 = -1;
    func_800C77C8();
    D_801476BC = 0;
    func_80055B14(selection_variant(&D_80142F18), selection_kind(&D_80142F18));
    if (func_80046240()) {
        if (D_801476C2) {
            func_80046C30(2);
            func_80048380();
            func_8007268C();
            return;
        }
        if ((((selection_flags(&D_80142F18) >> 2) & 1) ^ 1) != 0) {
            func_800C9800(0);
            if ((func_80094B98(D_80140160) ^ 1) != 0) {
                func_800C96FC();
                func_800C9870();
                func_80094B3C(D_80140160, 0);
            }
        } else {
            if ((func_800C9810() ^ 1) != 0) {
                func_800C9800(1);
            }
            func_80094B3C(D_80140160, 2);
        }
        return;
    }
    if ((((selection_flags(&D_80142F18) >> 2) & 1) ^ 1) != 0 || prev == 0xB) {
        func_800C9800(0);
        if ((func_80094B98(D_80140160) ^ 1) != 0) {
            func_800C96FC();
            func_800C9870();
            func_80094B3C(D_80140160, 0);
        }
    } else {
        if (func_800C9810()) {
            func_800C9870();
        } else {
            func_800C9800(1);
        }
        func_80094B3C(D_80140160, 2);
    }
    func_80046C30(2);
    func_80048380();
    func_800413E0();
    func_800B123C();
    func_800B2D50();
    func_800B2B50();
    obj = D_80142B10;
    obj->vtable->fn20((u8 *)obj + obj->vtable->delta20);
    func_800B6F58(obj);
    obj->vtable->fn28((u8 *)obj + obj->vtable->delta28);
    obj->vtable->fn30((u8 *)obj + obj->vtable->delta30);
    obj->vtable->fn38((u8 *)obj + obj->vtable->delta38);
    func_800B7340(obj);
    if (func_800B61EC(D_801476B8)) {
        D_80143434.opaque[2] = 1;
    }
    func_801F2108();
    func_800D7798();
    func_80122600();
    func_800C942C();
    func_8004569C();
    func_80049CB4(2);
    func_800C7830();
    func_800C92BC(D_801476B8);
    func_800413FC();
    func_80046374();
    func_80045E50();
    func_80049CB4(0xDB);
    func_80049CB4(2);
    if (D_801476C2) {
        func_8007268C();
        return;
    }
    func_80094FA8(D_80140160);
    func_80046B08(D_801476C2);
    func_800A9A90();
}
