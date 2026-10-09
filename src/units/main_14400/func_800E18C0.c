#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct Pos800E18C0 {
    s32 x;
    s32 y;
} Pos800E18C0;

typedef struct LifeMsg800E18C0 {
    s32 kind;
    u8 pad_04[0x14];
} LifeMsg800E18C0;

typedef struct LifeVTable800E18C0 {
    u8 pad_00[0x58];
    s16 adjust_58;
    s16 pad_5A;
    s32 (*message_5C)(void *self, LifeMsg800E18C0 *msg);
    s16 adjust_60;
    s16 pad_62;
    void (*refresh_64)(void *self);
    u8 pad_68[0x90 - 0x68];
    s16 adjust_90;
    s16 pad_92;
    s32 (*status_94)(void *self, s32 mode, s32 value, u8 byte, s32 extra);
} LifeVTable800E18C0;

typedef struct Unit800E18C0 {
    Pos800E18C0 pos_00;
    u8 pad_08[0x2];
    u8 kind_0A;
    u8 pad_0B[0x1E - 0xB];
    u8 flags_1E;
    u8 pad_1F[0x24 - 0x1F];
    LifeVTable800E18C0 *vtable_24;
    u8 pad_28[0x33 - 0x28];
    u8 speed_33;
    u8 pad_34;
    u8 status_35[10];
    u8 pad_3F[0x42 - 0x3F];
    u8 field_42;
    u8 field_43;
    u8 field_44;
    u8 pad_45[0x50 - 0x45];
    u16 field_50;
    u8 pad_52[0x70 - 0x52];
    s8 dx_70;
    s8 dy_71;
    u8 pad_72[0x75 - 0x72];
    u8 field_75;
} Unit800E18C0;

typedef struct Item800E18C0 {
    u8 field_00;
    u8 kind_01;
} Item800E18C0;

typedef struct Flags800E18C0 {
    u8 pad_00[0xE4];
    u16 flags_E4;
} Flags800E18C0;

extern u32 D_8013960C;
extern Flags800E18C0 *D_801476B8;
extern u16 D_801482CC[];
extern u16 D_80158C6C[];

s32 func_800A6FD0(Unit800E18C0 *self);
void func_800E2298(void *arg0);
s32 func_80049CB4(s32 id, ...);
u16 func_800E08B0(void *obj);
extern s32 func_800E0534(void *obj, s32 amount);
extern s32 func_800E1CD4(Unit800E18C0 *, s32);
s32 func_800E0F40(Unit800E18C0 *obj);
void func_800E43EC(Unit800E18C0 *, u16, u8);
u32 func_800E10D0(const Unit800E18C0 *record);
char *func_80048480(u16 id);
extern void func_800497F0(s32, ...);
s32 func_800A533C(u8 *a);
void *func_800B4D80(Pos800E18C0 *p);
void func_80124FBC(Item800E18C0 *item, Pos800E18C0 *pos);
u32 func_800E1148(const Unit800E18C0 *record);
char *func_800A3B20(Unit800E18C0 *u);
void func_800498E4(s32 id, ...);

static inline void copyPos(Pos800E18C0 *dest, Pos800E18C0 *src) {
    dest->x = src->x;
    dest->y = src->y;
}

static inline void setPos(Pos800E18C0 *dest, s32 x, s32 y) {
    dest->x = x;
    dest->y = y;
}

/* Clears status `status` of `unit` (life vtable slot 0x90, mode 1) and runs its expiry effects.
 * arg2/arg3 are supplied by the slot dispatcher func_800E115C (mode 1) but unused here. */
void func_800E18C0(Unit800E18C0 *unit, s32 status, u8 arg2, s32 arg3) {
    u16 message;

    if (status < 10 && unit->status_35[status] != 0) {
        unit->status_35[status] = 0;
        if (status == 0) {
            if (func_800A6FD0(unit) != 0) {
                func_800E2298(unit);
            }
        } else if (status == 6) {
            func_80049CB4(6);
            func_800E0534(unit, func_800E08B0(unit));
            func_80049CB4(7);
        } else if (status == 9) {
            s32 free = func_800E1CD4(unit, 0xF) != 1;

            if (free) {
                s32 kind = unit->kind_0A;

                func_800E43EC(unit, kind, func_800E0F40(unit));
            }
        }
    } else if (status < 0x11) {
        Pos800E18C0 pos;

        if (status == func_800E10D0(unit)) {
            unit->field_42 = 0;
            if (status == 0xF) {
                s32 name;
                u8 kind;
                s32 state;

                copyPos(&pos, &unit->pos_00);
                func_80049CB4(6);
                name = func_80049CB4(0x10FE, &pos);
                func_800497F0(0xA1, name, func_80048480(0x234));
                func_80049CB4(7);
                if (unit->vtable_24->status_94((u8 *)unit + unit->vtable_24->adjust_90, 2, 9, 0, 0) != 0) {
                    kind = 0xD5;
                    state = 1;
                } else {
                    if (unit->kind_0A == 0x31) {
                        LifeMsg800E18C0 msg;

                        msg.kind = 2;
                        unit->vtable_24->message_5C((u8 *)unit + unit->vtable_24->adjust_58, &msg);
                        state = unit->field_75;
                    } else {
                        state = func_800E0F40(unit);
                    }
                    kind = unit->kind_0A;
                }
                func_800E43EC(unit, kind, state);
                func_80049CB4(0x132);
                func_800A533C((u8 *)unit);
            } else if (status == 0xB) {
                D_8013960C <<= 1;
                unit->vtable_24->status_94((u8 *)unit + unit->vtable_24->adjust_90, 0, 0x12, 0xFE, 1);
                D_8013960C >>= 1;
            } else if (status == 0x10) {
                Item800E18C0 *item;

                setPos(&pos, unit->dx_70, unit->dy_71);
                item = func_800B4D80(&pos);
                if (item != 0 && item->kind_01 == 0xDB) {
                    func_80124FBC(item, &pos);
                }
            }
        }
    } else if (status < 0x13 && unit->field_43 != 0) {
        unit->field_50 = D_80158C6C[unit->speed_33];
        unit->field_43 = 0;
    } else if (status < 0x15) {
        if (status == func_800E1148(unit)) {
            unit->field_44 = 0;
        }
    } else {
        return;
    }
    func_80049CB4(0x1F, unit);
    func_80049CB4(0xA6, unit, status, 1);
    message = D_801482CC[status];
    if (status == 1 && ((D_801476B8->flags_E4 >> 3) & 1)) {
        message = 0;
    }
    if (message != 0 && (unit->flags_1E & 0xC)) {
        func_800498E4(message, func_800A3B20(unit));
    }
    unit->vtable_24->refresh_64((u8 *)unit + unit->vtable_24->adjust_60);
}
