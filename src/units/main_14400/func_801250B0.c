#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s32 x; s32 y; } Pos801250B0;
typedef struct { u8 value; } Dir801250B0;
typedef struct ShirenDirection { s8 value; } ShirenDirection;

/* Status word at +0x20 of a unit; bit 8 marks a state that ignores the hit. */
typedef union {
    u32 word;
    struct {
        u32 high : 23;
        u32 bit8 : 1;
        u32 low : 8;
    } bits;
} Status801250B0;

typedef struct Unit801250B0 Unit801250B0;
typedef struct {
    u8 pad00[0x10];
    s16 delta_10;
    s16 pad12;
    s32 (*method_14)(void *self);
    u8 pad18[0x68 - 0x18];
    s16 delta_68;
    s16 pad6A;
    s16 (*method_6C)(void *self);
} UnitVt801250B0;
struct Unit801250B0 {
    Pos801250B0 pos;
    u8 pad08[0x1E - 0x8];
    u8 flags_1E;
    u8 pad1F;
    Status801250B0 status_20;
    UnitVt801250B0 *vtable_24;
};

/* Line search filled by func_800C4360 / func_800C2D0C; +0x40 is the unit hit. */
typedef struct {
    u8 pad0[0x40];
    Unit801250B0 *target_40;
    u8 pad44[0xC];
} Path801250B0;

/* Damage record built by func_80136910. */
typedef struct {
    void *source;
    u32 kind;
    u32 field_8;
    u16 amount;
    u16 field_E;
    u8 field_10;
    u8 pad11[0x18 - 0x11];
} Damage801250B0;

extern u8 D_80147620[];
extern u8 D_80156A65;
extern u8 D_80156A67;
void *func_801156DC(void *out, void *owner, void *position, void *dir, s32 range);
void *func_800B31E8(void *pos, s32 team);
s32 func_80049CB4(s32 id, ...);
void *func_800C4360(void *self, void *owner, u16 value, s32 command, void *position, ShirenDirection direction, s32 limit, u16 flags, u8 mode);
void func_800C2D0C(Path801250B0 *path);
s32 func_800C5844(void *rng, u8 base, u8 top);
void *func_800C47A8(Path801250B0 *path);
u16 func_80115944(void *owner, u16 value);
void func_80136910(Damage801250B0 *obj, void *a, u32 c, u32 b, u32 e);
void func_800A7ADC(Unit801250B0 *entity, Damage801250B0 *damage);
void func_80049A04(u16 id, ...);
s32 func_800E0AB4(Unit801250B0 *obj, s32 amount);
char *func_800A3B20(Unit801250B0 *unit);
void func_800498E4(s32 id, ...);

/* ODD_C: the status word is copied whole and its bit read through this accessor; the copy
 * (one lw) and the accessor also shape the original's load scheduling. */
static inline s32 Status_bit8(Status801250B0 *status) {
    return status->bits.bit8;
}

/* ODD_C: constructor-style wrapper that builds the damage record and yields it for
 * func_800A7ADC; it also fixes the original's argument evaluation order. */
static inline Damage801250B0 *Damage_init(Damage801250B0 *damage, void *source, s16 amount) {
    func_80136910(damage, source, amount, 0x13, 8);
    return damage;
}

/* Slot 0x44 receives seven pointers; b, target and item are supplied but unused. */
s32 func_801250B0(void *owner, void *a, void *b, void *c, Dir801250B0 *facing, void *target, void *item) {
    Pos801250B0 pos;
    Path801250B0 path;
    Damage801250B0 damage;
    Dir801250B0 dir;
    Status801250B0 status;
    Path801250B0 *search;
    Unit801250B0 *unit;
    s32 mode;
    s32 repeat;

    dir.value = (facing->value - 2) & 7;
    func_801156DC(&pos, owner, c, &dir, 100);
    mode = func_800B31E8(c, 2) != 0 ? 0x24 : 4;
    func_80049CB4(0x125, &pos);
    search = &path;
    repeat = mode & 0x20;
    do {
        ShirenDirection back;

        back.value = (dir.value + 4) & 7;
        func_800C4360(search, a, 0xFB, 0x10E, &pos, back, 0xFF, mode, 0);
        func_800C2D0C(search);
        unit = path.target_40;
        if (unit != 0) {
            s32 power;
            void *source;

            pos = unit->pos;
            power = func_800C5844(D_80147620, D_80156A65, D_80156A67) & 0xFF;
            source = func_800C47A8(search);
            func_800A7ADC(unit, Damage_init(&damage, source, func_80115944(owner, power)));
            if (unit->vtable_24->method_14((char *)unit + unit->vtable_24->delta_10)) {
                if ((unit->flags_1E >> 2) & 1) {
                    break;
                }
            } else {
                status = unit->status_20;
                if (Status_bit8(&status)) {
                    func_80049A04(0xF9);
                } else if (unit->flags_1E & 0xC) {
                    func_800E0AB4(unit, -1);
                } else if (unit->flags_1E & 0x7C) {
                    s16 amount = unit->vtable_24->method_6C((char *)unit + unit->vtable_24->delta_68);

                    if (amount >= 2) {
                        func_800E0AB4(unit, -amount);
                        func_800498E4(0x24, func_800A3B20(unit));
                    }
                }
            }
        }
    } while (repeat && unit != 0);
    return 1;
}
