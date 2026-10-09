#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Pos;
typedef struct { u8 x, y, amount, flags; } Record;
typedef struct { u32 bits; } Flags;
typedef struct { u8 pad00[0x18]; short adjust18, pad1A; s32 (*test1C)(void *, s32); } ItemTable;
typedef struct { u8 pad00[8]; ItemTable *vtable08; } Item;
typedef struct { u8 pad00[0x10]; short adjust10, pad12; s32 (*test14)(void *); } EntityTable;
typedef struct { Pos position; u8 byte08, flags09; u8 pad0A[0x12]; u16 flags1C; u8 flags1E, pad1F; Flags flags20; EntityTable *vtable24; } Obj;
typedef struct { void *source; u32 kind, field08; u16 value0C, flags0E; u8 mode10, pad11[7]; } Damage;
extern u8 D_80143393;
extern Record D_80143394[40];
extern u16 D_801569FC;
extern u32 func_800B1C6C(Pos *pos);
extern void *func_800B4D80(Pos *pos);
extern Record *func_800B51D4(Obj *obj);
extern s32 func_800A23E8(Pos *origin, Pos *vec);
extern s32 func_80049CB4(s32 id, ...);
extern s32 func_800B56F0(void *object);
extern char *func_800AE674(void *obj);
extern void func_800497F0(s32 id, ...);
extern void func_800D3650(void *arg);
extern void func_800B4E7C(Pos *pos);
extern void *func_800B4928(Pos *pos);
extern void func_80136910(Damage *damage, void *source, u32 value, u32 kind, u32 flags);
extern char *func_800A3B20(Obj *obj);
extern void func_800A7ADC(Obj *obj, Damage *damage);

/* ODD_C: Addressable value helpers preserve the original copies and promotions. */
static inline Flags *copy_flags(Flags *copy, Flags value) { *copy = value; return copy; }
static inline s32 flagged(Flags *flags) { return (flags->bits >> 25) & 1; }
static inline s32 category(Obj *obj) { return obj->flags09 & 15; }
static inline u16 cap_amount(u16 value) { return value > 100 ? 100 : value; }
static inline s32 source_flag(Obj *source) {
    s32 flag = 0;
    if (source) flag = (source->flags1E & 0xC) != 0;
    return flag;
}
static inline void init_damage(Damage *damage, void *source) {
    func_80136910(damage, source, (short)D_801569FC, 0x22, 0x808);
}

s32 func_800B5300(Obj *self, Obj *source, u8 amount)
{
    Pos at, origin;
    Damage damage;
    Flags flags;
    Item *item;
    Record *record;
    Obj *entity;
    s32 message;
    s32 valid;
    u16 total;
    if (func_800B1C6C(&self->position) & 0xE100) return 0;
    item = func_800B4D80(&self->position);
    if (item && item->vtable08->test1C((char *)item + item->vtable08->adjust18, 0x24)) return 0;
    record = func_800B51D4(self);
    if (record == 0) {
        s32 best = 0;
        Record *entry = D_80143394;
        s32 remaining = 40;
        for (;;) {
            s32 distance;
            if (--remaining == -1) break;
            if (!entry->amount) continue;
            at.y = entry->x;
            at.x = entry->y;
            origin.x = self->position.x;
            origin.y = self->position.y;
            distance = func_800A23E8(&at, &origin);
            if (best < distance) { best = distance; record = entry; }
            entry++;
        }
        record->amount = 0;
        D_80143393--;
        at.y = record->x;
        at.x = record->y;
        func_80049CB4(0xD7, &at);
    }
    func_80049CB4(0x1131);
    valid = func_800B56F0(self) != 1;
    if (valid) {
        func_80049CB4(6);
        message = func_80049CB4(0x119, self);
        func_80049CB4(7);
    } else message = func_80049CB4(0xDA, self);
    func_80049CB4(0xD7, self);
    total = record->amount;
    if (!total) D_80143393++;
    total = cap_amount(total + amount);
    record->x = (u8)self->position.y;
    record->y = (u8)self->position.x;
    if (!record->amount) record->flags = source_flag(source);
    record->amount = total;
    if (item) {
        func_80049CB4(6);
        func_800497F0(0x106, message, func_800AE674(item));
        func_80049CB4(7);
        func_800D3650(item);
        func_800B4E7C(&self->position);
    }
    entity = func_800B4928(&self->position);
    valid = 0;
    if (entity && !entity->vtable24->test14((char *)entity + entity->vtable24->adjust10)) {
        if (category(entity) >= 2 && !(entity->flags1C & 1)) valid = !flagged(copy_flags(&flags, entity->flags20));
    }
    if (valid) {
        init_damage(&damage, source);
        if (entity->flags1E & 0x7C) {
            func_80049CB4(6);
            func_800497F0(0x103, message, func_800A3B20(entity));
            func_80049CB4(7);
        }
        func_80049CB4(6);
        func_800A7ADC(entity, &damage);
        func_80049CB4(7);
    }
    func_80049CB4(0x12D);
    return 1;
}
