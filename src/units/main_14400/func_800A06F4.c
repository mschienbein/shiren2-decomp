#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef struct { s32 x; s32 y; } Position;
typedef struct { s8 x; s8 y; } Offset;
typedef struct { u8 pad00[0x10]; s16 delta10; s16 pad12; s32 (*test14)(void *); } VTable;
typedef struct { u8 pad00[0x1C]; u16 flags1C; u8 flags1E; u8 pad1F[5]; const VTable *vtable24; } Entity;
typedef struct { void *source; u32 kind; u32 field08; s16 amount; u16 flags; u8 phase; u8 pad11[3]; s32 field14; } Damage;
extern const Offset D_801428F0[9];
extern s32 func_80049CB4(s32 id, ...);
extern u32 func_800B1C6C(Position *position);
extern void func_800B4788(void *position);
extern void func_80136910(Damage *damage, void *source, u32 amount, u32 kind, u32 flags);
extern s32 func_800C94D8(void);
extern Entity *func_800B4928(Position *position);
extern s32 func_800A58B8(Entity *entity);
extern void func_800A7ADC(Entity *entity, Damage *damage);

static inline u32 tile_flag(Position *position)
{
    return func_800B1C6C(position) & 0x100;
}

static inline Position *offset_position(Position *out, Position *origin, s32 dx, s32 dy)
{
    s32 x = origin->x + dx;
    s32 y = origin->y + dy;
    out->x = x;
    out->y = y;
    return out;
}

void func_800A06F4(Position *origin, u16 amount, Entity *source, s32 kind, s32 includeSource)
{
    Damage damage;
    Position position;
    s32 clear;
    s32 index;
    s32 firstHit;
    func_80049CB4(0x115, origin);
    clear = 0;
    if (!(func_800B1C6C(origin) & 0x2000))
        clear = tile_flag(origin) == 0;
    if (clear)
        func_800B4788(origin);
    /* The damage constructor receives the low 16 bits of amount sign-extended. */
    func_80136910(&damage, source, (u32)(s32)(s16)amount, (u32)kind, 0x408);
    index = 9;
    firstHit = 1;
    for (;;) {
        Entity *entity;
        s32 eligible;
        if (--index == -1 || (func_800C94D8() ^ 1) != 0)
            break;
        entity = func_800B4928(offset_position(&position, origin, D_801428F0[index].x, D_801428F0[index].y));
        eligible = 0;
        if (entity && (source != entity || includeSource) && func_800A58B8(entity) == 2) {
            if (!entity->vtable24->test14((u8 *)entity + entity->vtable24->delta10) && !(entity->flags1C & 1))
                eligible = 1;
        }
        if (eligible) {
            if (entity->flags1E & 0x7C) {
                if (firstHit)
                    entity->flags1C |= 0x100;
                func_800A7ADC(entity, &damage);
                if (firstHit)
                    entity->flags1C &= ~0x100;
                firstHit = 0;
            } else {
                func_800A7ADC(entity, &damage);
            }
        }
    }
}
