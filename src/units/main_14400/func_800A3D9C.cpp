#include "common.h"
#include "map_types.h"
#include "item_records.h"
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

/* Shared value types for the private Creature translation unit.  This is a
 * generator input, not an additional translation unit or a new include file.
 * Position's explicit copy construction and implicit copy assignment are
 * deliberately different operations, as evidenced by A694C and A7204. */
/* Map coordinates and directions are the shared MapPos/MapDir of map_types.h. */
/* Trivially constructible scalar working rectangle for B1F58/A33DC (A5B98/A5E24/A5FCC).
 * r3 types this as map_rect.h MapRect, which in C++ is not trivially constructible:
 * A5FCC's ROM rematerializes its address (0x800A5FE0, 0x800A6018), so the local is not
 * that class here; the corner order x0,y0,x1,y1 is MapRect's min/max. */
struct Bounds { s32 values[4]; };
/* Map rectangle: two MapPos corners, returned by value (B3024/B3080) and stored in
 * Creature+0x0C and D_801429C0. The class-specifier is token-identical to r3's
 * include/map_rect.h; that header is not included because its B1F58(MapRect *)
 * declaration cannot describe the trivially constructible working rectangle above. */
struct MapRect { MapPos min; MapPos max; };

/* Partial source-layout views: named fields have producer/consumer evidence.
 * No class is enlarged to the 0xE4 allocation stride merely to fill a slot. */
struct Creature;
struct Damage;
struct Outcome { u16 flags; u16 amount; };
struct Inventory;
struct CreatureMessage;
struct Room;

/* One physical flag word: A8078..A80D0 operate on runtime masks; the
 * reviewed consumers also use unsigned one-bit fields. ECC74 independently
 * uses both access styles in one union. Field order is target big-endian.
 * Rest fields carry unconstrained bits, not zero padding or new meanings. */
union UnitFlags {
    u32 raw;
    struct {
        u32 rest26_31 : 6;
        u32 b25 : 1;
        u32 b24 : 1;
        u32 rest23 : 1;
        bool b22 : 1;   /* flag bit 22 (A455C.c: "flying"); a C++ bool flag bit */
        u32 rest14_21 : 8;
        u32 b13 : 1;
        u32 b12 : 1;
        u32 rest00_11 : 12;
    } bits;
    u32 bit24() const { return bits.b24; }
    u32 bit13() const { return bits.b13; }
    u32 bit12() const { return bits.b12; }
    /* Preserve A6EE0's existing whole-word predicates, not the rejected
     * bitfield/union variants from its earlier isolated sweeps. */
    u32 bit17() const { return (raw >> 17) & 1; }
    u32 bit18() const { return (raw >> 18) & 1; }
    u32 test(u32 mask) const { return raw & mask; }
    bool canReflect() const { return bits.b13; }
    bool isSilent() const { return bits.b25; }
};
struct DestroyEntry { s16 delta, index; void (*fn)(Creature *, s32); };
struct QueryEntry { s16 delta, index; s32 (*fn)(Creature *); };
struct ResetEntry { s16 delta, index; void (*fn)(Creature *); };
struct DescribeEntry { s16 delta, index; char *(*fn)(Creature *, char *); };
struct PlaceEntry { s16 delta, index; s32 (*fn)(Creature *, MapPos *, s32); };
struct RelationEntry { s16 delta, index; s32 (*fn)(Creature *, Creature *, u8 *); };
struct BoundsEntry { s16 delta, index; s32 (*fn)(Creature *, MapPos *); };
struct ModeEntry { s16 delta, index; s32 (*fn)(Creature *, s32); };
struct MessageEntry { s16 delta, index; s32 (*fn)(Creature *, CreatureMessage *); };
struct ConditionEntry { s16 delta, index; s32 (*fn)(Creature *, s32, s32, u8, s32); };
struct InventoryEntry { s16 delta, index; Inventory *(*fn)(Creature *); };
struct CreatureVTable {
    u8 unknown00[8];
    DestroyEntry destroy08;
    QueryEntry query10;
    ResetEntry reset18;
    u8 unknown20[0x10];
    DescribeEntry describe30;
    PlaceEntry place38;
    RelationEntry relation40;
    BoundsEntry bounds48;
    ModeEntry mode50;
    MessageEntry message58;
};
/* The ROM proves the base D_801535E8 table does NOT have slots 90/98.
 * Only the respective derived tables are viewed through these extensions. */
struct LivingVTable : CreatureVTable {
    u8 unknown60[0x30];
    ConditionEntry condition90;
};
struct InventoryVTable : LivingVTable { InventoryEntry inventory98; };
struct Creature {
    MapPos pos;
    MapDir dir;
    u8 mode09;
    u8 kind;
    u8 unknown0B;
    /* A5A04 assigns a returned rectangle; A6B70 passes it to A694C. */
    MapRect bounds;
    u16 state;
    u8 tag;
    u8 baseKind;
    UnitFlags flags;
    CreatureVTable *vt;
    /* g++ 2.x virtual-this adjustment, not an erased call contract. */
    s32 isGone() { return vt->query10.fn((Creature *)((char *)this + vt->query10.delta)); }
    void vanish() { vt->reset18.fn((Creature *)((char *)this + vt->reset18.delta)); }
};
/* Only the +75 field is accessed here; the 28..74 interval is unresolved. */
struct LivingCreature : Creature { u8 unknown28[0x4D]; u8 field75; };
/* Class tag 0x10, established by EFC70. The 8C value is consumed as an
 * inventory object pointer by DAC60, rather than as an integer carrier. */
struct InventoryCreature : LivingCreature {
    u8 unknown78[0x14];
    Inventory *inventory8C;
    u8 unknown90[6];
    u16 name96;
};
/* DAC60 proves the vptr at +4, not a discriminator at +0.
 * All four prefix bytes, the full extent, and the exact kind-5A inline
 * dynamic subclass remain unknown. CEC90's element kind is not this object. */
struct InventoryVptr;
struct Inventory { u8 unknown00[4]; InventoryVptr *vt; };
struct InlineInventoryCreature : LivingCreature {
    u8 unknown78[0x14];
    Inventory inventory8C;
};
/* EB120 establishes tag bit 2. No fictitious E4-sized common base is used. */
struct Player : LivingCreature {
    u8 unknown78[0x6C];
    u16 flagsE4;
    u8 unknownE6[0x1E];
    Creature *controlled104;
    s32 isProtected() const { return (flagsE4 >> 3) & 1; }
};
/* 32-byte area-unit iterator (r3 MapUnitIterator layout: cursor, excluded, center,
 * area). A915C/A9204 initialize it. It stays trivially constructible: A4954's ROM
 * rematerializes its address (0x800A4980, 0x800A49B0), which r3's C++ MapUnitIterator
 * (MapPos/MapRect members) does not reproduce. */
struct UnitIterator { s32 cursor; void *excluded; s32 center[2]; s32 area[4]; };
struct ScanDir { s8 value; };
/* r3 map_line_iterator.h / map_ray_iterator.h / map_ring_iterator.h layouts and names. */
struct PathIterator { MapPos current; u8 direction; s32 length; s32 index; };
struct RayIterator { PathIterator line; MapPos origin; s32 available; u32 mask; };
struct RadialIterator {
    MapPos center;
    u16 radius;
    u16 limit;
    s16 offsetY;
    s16 offsetX;
    u8 direction;
    u16 steps;
};
struct ActionVTable;
struct HitCommand { s16 kind, field02; s32 field04, field08; ActionVTable *vt; u16 value10; };
struct MoveAction { Creature *owner; s16 flags; s32 count; ActionVTable *vt; HitCommand *command; };
extern "C" void func_80136910(Damage *, Creature *, u32, u32, u32);
/* Damage record built only by func_80136910 (+0x00 source, +0x04 kind, +0x08 word 0,
 * +0x0C amount, +0x0E flags, +0x10 byte 0xA), always into a caller stack slot (42 call
 * sites); it reaches the +0x58 creature handlers through the kind 8/9/10 messages of
 * func_800A7A9C/7ADC/7B68, whose consumers access +0x00, +0x04, +0x0B, +0x0C, +0x0E and
 * +0x10 only. No copy, array, heap allocation or member placement exists, and the
 * 8-byte rounding of gcc 2.8 BLKmode stack slots admits any size 0x11..0x18, so the
 * members below are the complete evidenced record (natural size 0x14). */
struct Damage {
    Creature *source;
    u32 kind;
    u32 field08;
    u16 amount;
    u16 flags;
    u8 field10;
    Damage() {}
    Damage(Creature *from, s16 value) { func_80136910(this, from, value, 0x22, 0x808); }
};
/* Real tagged payload: E4B60 dispatches on kind and reads +10 differently.
 * This union is not a family of aliases for unrelated whole objects. */
struct CreatureMessage {
    s32 kind;
    Creature *source;
    Creature *target;
    u32 field0C;
    union { Damage *damage; s32 value; } payload;
};
struct ResultMessage : CreatureMessage { Outcome *result; };
/* Trap message for an item's handler slot +0x38 (ItemMethods::handle3C). For trap items
 * (kind 0x10, the base table and its 25 derived tables, wave29 creature-r6
 * trap-bindings.json) the slot is func_80116468, which forwards kinds 0x15/0x16/0x17 with
 * the same pointer to func_801159CC: +0x0C direction and +0x10 position (0x15/0x17), +0x08
 * object (0x16/0x17) and the word at +0x18 (0x16). func_800C33FC sends kinds 0x0E and 0x17
 * from one local, so the record is shared by kind. +0x18 is the last trap-consumer
 * field for these kinds. The record is the one 0x20-byte item message the handle3C
 * senders allocate (func_800C37B8 and func_800C33FC send kinds 0x15/0x17 with it): 60 of
 * the 67 stack send sites, kind-only senders such as func_800AFEB4 included, place the
 * next object exactly at +0x20, and other kinds write and read +0x1C (func_800C37B8,
 * func_800AF28C, func_80120D88; halfword +0x1E in func_8011D6C0). A7A1C's local therefore
 * fills its 0x20-byte frame slot (frame 0x38). The kind word is item_records.h's
 * ItemMessage base, embedded first as the header requires of every sender. */
struct TrapMessage { ItemMessage base; u8 unknown04[8]; MapDir direction; MapPos pos; s32 field18; s32 field1C; };
/* Serialisation stream of func_800CA4A4/func_800CA4E8 (vptr +0x18). Slot +0x18 writes and
 * slot +0x28 reads (self, size, buffer). The only stream reaching the creature save/load
 * slots (+0x20/+0x28 of D_801535E8 and the Tag2 tables, called by func_800C9C00/C9CC4 and
 * func_800A8D04/A8E40) is D_8014769C, built by func_80044620 for func_800CA76C: the
 * device stream (table D_8014A840, ctor func_800439A8: write func_80043FD0, read
 * func_80044154) or the read-only stream (table D_8014A8E8, ctor func_80044308: write
 * func_8004449C, empty; read func_800444A4). All four entries have delta 0. */
struct SaveStream;
struct SaveStreamEntry { s16 delta, index; void (*fn)(SaveStream *, s32, void *); };
struct SaveStreamVTable { u8 unknown00[0x18]; SaveStreamEntry write18; u8 unknown20[8]; SaveStreamEntry read28; };
struct SaveStream { u8 unknown00[0x18]; SaveStreamVTable *vt; };
/* These two two-word copies have no proven Position callers. Keep their
 * original independent record identities instead of folding by sizeof. */
struct CopyRecord8058 { s32 unk0, unk4; };
struct CopyRecord80EC { s32 field_0, field_4; };
struct ItemInfo { u8 field00, field01, field02; };

typedef struct { u8 priority; MapDir direction; } Assessment_800A4954;
typedef struct { u8 pad[3]; u8 field3; } Tile_800A578C;
typedef s32 bool32_800A6184;
typedef struct {
    u8 dir;
    u8 diff;
} Turn_800A6690;
struct Damage;
extern "C" {
extern Player *D_801476B8;
extern MapRect D_801429C0;
extern u8 D_80147620[];
extern const char D_801535DC[];
extern char D_801429FC[];
extern signed char D_801429E8[][5];
extern u32 D_8013960C;
extern u8 D_8014344C;
extern unsigned short D_8015691C;
extern unsigned short D_8015698C;
void func_80046B3C(Creature *, MapDir);
s32 func_800A23E8(MapPos *, MapPos *);
s32 func_800A24DC(MapPos *, MapRect *);
s32 func_800A251C(MapPos *, MapPos *);
MapPos *func_800A2594(MapPos *, MapPos *, MapDir);
void func_800A2758(MapPos *, MapDir);
MapDir *func_800A27A4(MapDir *, MapPos *, MapPos *);
MapDir *func_800A27E8(MapDir *, MapPos *, MapPos *);
s32 func_800A282C(MapPos *, MapPos *);
void func_800A2F80(u8 *, s32);
void func_800A2F94(u8 *, s32);
MapPos *func_800A33DC(MapPos *, Bounds *);
MapPos *func_800A39C0(MapPos *, Creature *, u8 *, s32);
char *func_800A3B20(Creature *);
void func_800A0618(Creature *);
UnitIterator *func_800A915C(UnitIterator *, void *);
UnitIterator *func_800A9204(UnitIterator *, MapRect *, void *);
s32 func_800A9284(UnitIterator *, s32);
void *func_800A942C(UnitIterator *);
u32 func_800B1C6C(MapPos *);
s32 func_800B1DF8(MapPos *);
s32 *func_800B1F58(s32 *);
Room *func_800B1F90(MapPos *);
s32 func_800B20B4(Creature *, MapPos *);
MapPos *func_800B221C(MapPos *);
MapRect func_800B3024(void *);
MapRect func_800B3080(void *);
s32 func_800B4888(MapPos *);
s32 func_800B48C0(MapPos *, Creature *);
Creature *func_800B4928(MapPos *);
Creature *func_800B49B8(MapPos *);
Item *func_800B4D80(MapPos *);
s32 func_800B4F74(MapPos *);
s32 func_800B502C(MapPos *);
s32 func_800B56F0(MapPos *);
u16 func_800B5768(MapPos *);
s32 func_800B5900(void *, s32, s32, void **);
s32 func_800B5BDC(MapPos *);
void func_800C25D0(PathIterator *, const MapPos *, const u8 *, s32);
MapPos *func_800C2758(MapPos *, PathIterator *);
void func_800C4864(MoveAction *, s32, MapPos *);
MoveAction *func_800C4DA0(MoveAction *, Creature *, HitCommand *, u16);
HitCommand *func_800C4F34(HitCommand *, u16);
void *func_800C5150(RayIterator *, MapPos *, ScanDir, s32, u32);
void *func_800C51B8(RayIterator *);
RadialIterator *func_800C5280(RadialIterator *, MapPos *, u16);
MapPos *func_800C532C(MapPos *, RadialIterator *);
s32 func_800C559C(RadialIterator *);
Creature *func_800C5F60(void);
void func_800CA4A4(SaveStream *, const char *);
void func_800CA4E8(SaveStream *, const char *);
u16 func_800E08B0(Creature *);
u16 func_800E08F0(Creature *);
s32 func_800E0F40(Creature *);
s32 func_800E1CC4(Creature *, s32);
s32 func_800E1CD4(Creature *, s32);
s32 func_800E2074(Creature *);
void func_800E2298(Creature *);
void func_800E370C(Creature *, s8 *);
s32 func_800E4454(Creature *);
int func_800EC630(Creature *, ItemInfo *);
void func_80136910(Damage *, Creature *, u32, u32, u32);
s32 func_800F4810(u8 kind);
char *func_80048480(u16 id);
s32 func_800F4044(s32 id, u8 b);
s32 func_800327C0(char *dst, const char *fmt, ...);
u32 func_80048AC8(void);
extern unsigned char func_800C57CC(void *, s32);
u8 func_800C57A0(void *rng);
extern s32 func_80049CB4(s32 id, ...);
s32 func_800C587C(void *a, u8 b);
extern s32 func_8005DFE8(unsigned char *);
extern void func_80049AE8(s32, ...);
extern void func_800497F0(s32, ...);
extern Tile_800A578C *func_800B51D4(MapPos *);
s32 func_800B58E4(s32 a, s32 b, s32 c, s32 d);
extern void *func_800B31E8(void *object, s32 code);
extern s32 func_801F258C(s32, s32);
void func_800A0600(void);
extern s32 func_800A3A30(unsigned char type);
char *func_800A3D9C(Creature *unit);
s32 func_800A4010(Creature *unit, u16 flags, s32 mode);
s32 func_800A4124(s32 flags, s32 mode);
s32 func_800A4140(s32 flags, s32 state);
s32 func_800A416C(s32 flags, s32 mode);
s32 func_800A4198(s32 flags, s32 type);
s32 func_800A41B4(s32 arg0, s32 arg1);
s32 func_800A41D0(s32 flags, s32 mode);
s32 func_800A41EC(Creature *arg, MapPos *value);
s32 func_800A422C(Creature *o, MapPos *a, s32 b);
s32 func_800A42CC(Creature *a, MapPos *b, s32 mode);
s32 func_800A4314(Creature *object, MapPos *position);
s32 func_800A4360(Creature *obj, MapPos *arg1);
s32 func_800A4404(Creature *state, MapPos *position, void **output);
s32 func_800A44E8(Creature *obj, Creature *target, u8 *out);
s32 func_800A44F4(Creature *self, Creature *target);
s32 func_800A4520(Creature *ctx, Creature *obj);
s32 func_800A455C(Creature *obj, Creature *dest, s32 maxSteps);
s32 func_800A46BC(Creature *obj, MapPos *arg, MapDir *cell);
s32 func_800A4754(Creature *self, MapPos *pos, MapDir *dir);
Creature *func_800A492C(Creature *a, s32 b, s32 c, s32 d);
Creature *func_800A4954(Creature *object, s32 category, u8 *out_priority, s32 use_priority, s32 check_target);
Creature *func_800A4B24(Creature *p, s32 expected, s32 ranked, s32 checked);
s32 func_800A4CC4(Creature *self, MapPos *pos, MapDir *dir);
s32 func_800A4E30(Creature *position, MapDir *direction);
void func_800A4EC0(Creature *state, MapDir *value);
s32 func_800A4EFC(Creature *p, MapDir *a);
void func_800A4F58(Creature *a, MapDir *b);
s32 func_800A4FBC(Creature *a, MapDir *b);
s32 func_800A5018(Creature *self, s32 kind);
s32 func_800A50AC(Creature *object);
s32 func_800A50E8(Creature *a);
s32 func_800A5168(Creature *self, MapPos *position, s32 alternate);
s32 func_800A529C(Creature *obj, s32 flag);
s32 func_800A533C(Creature *a);
s32 func_800A5388(Creature *obj, s32 input_mask);
s32 func_800A5440(Creature *object, Creature *target, u8 *direction, s32 kind, char *item);
void func_800A56D8(Creature *a, MapPos *b, s16 c, u16 d);
s32 func_800A5758(Creature *a, MapPos *b, s16 c, u16 d);
void func_800A578C(Creature *p);
s32 func_800A58B8(Creature *self);
void func_800A58FC(Creature *actor, MapPos *position);
void func_800A592C(Creature *a, MapPos *b);
void func_800A59A4(Creature *self);
void func_800A5A04(Creature *object);
void func_800A5A70(Creature *object, u32 value);
void func_800A5A88(Creature *obj, s32 value);
u8 func_800A5AA0(Creature *self);
s32 func_800A5AE8(Creature *u, MapPos *pos);
s32 func_800A5B98(Creature *o, MapPos *out);
s32 func_800A5D2C(Creature *object, MapPos *output, s32 flags);
s32 func_800A5DC4(Creature *self, MapPos *position, s32 flags);
s32 func_800A5E24(Creature *self, MapPos *out, s32 sameRoom, s32 occupied);
s32 func_800A5FCC(Creature *obj, MapPos *out);
s32 func_800A60D8(Creature *obj, MapPos *out);
s32 func_800A6184(Creature *self, MapPos *pos);
s32 func_800A6218(Creature *self, MapPos *position, s32 notify);
s32 func_800A6348(Creature *self, MapPos *pos);
s32 func_800A63FC(Creature *a, MapPos *b);
u8 func_800A6420(Creature *obj,Creature *target);
s32 func_800A650C(Creature *origin, MapPos *vector);
MapDir *func_800A6538(MapDir *out_direction, Creature *obj, MapPos *target);
s32 func_800A65B8(Creature *object, MapPos *value);
MapDir *func_800A65E4(MapDir *p, Creature *q, MapPos *target);
MapDir *func_800A6620(MapDir *out, MapPos *from, MapPos *p);
void func_800A665C(Creature *obj, MapDir *value);
void func_800A6690(Creature *obj, MapDir *target, s32 arg);
s32 func_800A674C(Creature *self, Creature *other);
s32 func_800A67DC(Creature *u, Creature *o, s32 force, s32 apply);
s32 func_800A692C(Creature *object, s32 code);
MapPos func_800A694C(void *center, MapRect *bounds, u8 filter, const s32 check);
MapPos func_800A6B70(void *source, u8 mode, s32 arg);
Creature *func_800A6BA4(Creature *self, s32 count, s32 flag);
MapPos *func_800A6CC0(MapPos *obj, Creature *source);
Creature *func_800A6CF0(Creature *self);
Item *func_800A6D18(Creature *object);
Creature *func_800A6D40(Creature *p);
Item *func_800A6DC8(Creature *u);
Inventory *func_800A6DE4(Creature *obj);
int func_800A6E50(Creature *arg0,
                  ItemInfo *arg1);
s32 func_800A6E90(Creature *p);
s32 func_800A6EE0(Creature *obj);
s32 func_800A6F58(Creature *self, s32 mode);
s32 func_800A6F98(Creature *object, s32 mode);
s32 func_800A6FD0(Creature *self);
s32 func_800A7024(Creature *object);
MapPos *func_800A7044(MapPos *out, Creature *unit, MapDir *dir, s32 range, s32 flag);
void func_800A7204(Creature *u, Creature *attacker, MapDir *facing, s32 range, s32 damage, s32 kind, s32 checkWall, s32 notify);
s32 func_800A7824(Creature *unit, u8 steps);
s32 func_800A7A1C(Creature *place);
void func_800A7A9C(Creature *obj, Damage *message, Outcome *result);
void func_800A7ADC(Creature *entity, Damage *damage);
void func_800A7B18(Creature *target, Creature *source, s32 amount, s32 kind);
void func_800A7B68(Creature *object, Damage *damage);
void func_800A7BA4(Creature *obj, s32 value);
s32 func_800A7BE4(Creature *arg, Creature *source);
void func_800A7C1C(Creature *obj);
void func_800A7C54(Creature *self, SaveStream *obj);
void func_800A7CAC(Creature *arg0, SaveStream *obj);
s32 func_800A7D04(Creature *object, s32 mask);
s32 func_800A7D14(Creature *arg0);
s32 func_800A7D20(Creature *object);
s32 func_800A7D30(Creature *p);
s32 func_800A7D40(Creature *self);
s32 func_800A7D50(Creature *object);
s32 func_800A7D60(Creature *obj);
s32 func_800A7D70(Creature *object);
s32 func_800A7D80(Creature *item);
s32 func_800A7D9C(Creature *obj);
s32 func_800A7DAC(Creature *item);
s32 func_800A7DBC(Creature *object);
u8 func_800A7DCC(Creature *obj);
unsigned char func_800A7DD4(Creature *record);
MapRect *func_800A7DDC(Creature *obj);
MapDir *func_800A7DE4(Creature *obj);
MapPos *func_800A7DEC(Creature *value);
s32 func_800A7DF4(s32 arg0, s32 arg1);
UnitFlags *func_800A7DFC(UnitFlags *result, Creature *object);
void func_800A7E0C(Creature *object, MapPos *target, s32 mode);
void func_800A7E68(Creature *object, MapPos *target_position, s32 mode);
void func_800A7EC4(Creature *p, MapPos *target);
void func_800A7F0C(Creature *self, MapPos *target);
void func_800A7F54(Creature *obj);
s32 func_800A7F84(Creature *obj);
s32 func_800A7FCC(Creature *p, MapDir *dir);
s32 func_800A800C(Creature *arg, MapDir *dir);
s32 func_800A804C(Creature *obj);
void func_800A8058(CopyRecord8058 *arg0, CopyRecord8058 *arg1);
void func_800A806C(Creature *p, s32 v);
void func_800A8078(Creature *state, u32 mask);
void func_800A808C(Creature *p, s32 mask);
void func_800A80A0(Creature *object, u32 mask);
void func_800A80B0(Creature *record, u32 flags);
void func_800A80C0(Creature *obj, u32 value);
void func_800A80C8(Creature *record, u32 value);
void func_800A80D0(Creature *obj);
void func_800A80D8(Creature *obj, s32 value);
void func_800A80E0(Creature *obj, s32 packed);
void func_800A80EC(CopyRecord80EC *destination, const CopyRecord80EC *source);
void func_800A8100(Creature *obj, s32 value);
void func_800A8108(Creature *a, u8 v);
char *func_800A8114(Creature *self, char *buffer);
static inline s32 Flags_locked_800A4010(UnitFlags *flags) { return flags->bit24(); }

static inline s32 Flags_locked_800A4360(UnitFlags *flags) { return flags->bit24(); }

static inline s32 Obj_locked_800A4360(Creature *obj) {
    UnitFlags flags = obj->flags;
    return Flags_locked_800A4360(&flags);
}

static inline u16 Obj_status_800A4360(Creature *obj) { return obj->state; }

static inline s32 Flags_diagonalOk_800A4754(UnitFlags *f){ return f->bit24(); }

static inline MapDir Dir_make_800A4754(s32 v){ MapDir d; d.value = v; return d; }

static inline s32 terrain_is_water_800A4754(u32 t) { if (t & 0x2000) return 1; return 0; }

static inline s32 iterator_remaining_800A4B24(RayIterator *iterator) {
    return iterator->available;
}

static __inline__ MapPos *assign_position_800A5168(MapPos *out, const MapPos *in) {
    *out = *in;
    return out;
}

static inline void position_800A529C(MapPos *out, Creature *unit) {
    out->x = unit->pos.x;
    out->y = unit->pos.y;
}

static inline s32 Flags_bit13_800A5388(UnitFlags *flags) { return flags->bit13(); }

static inline s32 Flags_bit12_800A5388(UnitFlags *flags) { return flags->bit12(); }

static inline void copy_point_800A578C(MapPos *out, MapPos *in) { out->x = in->x; out->y = in->y; }

static inline void create_event_800A578C(Damage *out, Creature *owner, s16 id) { func_80136910(out, owner, id, 34, 0x808); }

static inline UnitFlags *copy_flags_800A578C(UnitFlags *out, const UnitFlags *in) { *out = *in; return out; }

static inline s32 variant_800A5A04(Creature *object) {
    return (object->tag >> 1) & 1;
}

static inline s32 canPlace_800A5B98(s32 r) { return r == 1; }

static inline void rect_set_800A5B98(Bounds *r, s32 x0, s32 y0, s32 x1, s32 y1) {
    r->values[0] = x0;
    r->values[1] = y0;
    r->values[2] = x1;
    r->values[3] = y1;
}

static inline void copy_coord_800A5E24(MapPos *dst, MapPos *src) {
    dst->x = src->x;
    dst->y = src->y;
}

static inline void pick_coord_800A5E24(Creature *self, Bounds *bounds, MapPos *out) {
    MapPos pick;
    if (self->state & 8) {
        func_800A33DC(&pick, bounds);
    } else {
        func_800B221C(&pick);
    }
    *out = pick;
}

static inline s32 coord_distance_800A5E24(MapPos *from, MapPos *to) {
    MapPos target;
    copy_coord_800A5E24(&target, to);
    return func_800A23E8(from, &target);
}

static inline s32 in_room_800A5E24(void *room, MapPos *coord) {
    return room == func_800B1F90(coord);
}

static inline s32 is_occupied_800A5E24(MapPos *coord) {
    return func_800B5BDC(coord) != 0;
}

static inline void copy_pos_800A60D8(MapPos *dst, MapPos *src) { dst->x = src->x; dst->y = src->y; }

static inline MapPos *copy_point_800A6218(MapPos *dest, MapPos *src) { dest->x = src->x; dest->y = src->y; return dest; }

static inline void copyPosition_800A6420(MapPos *dest, MapPos *src) { dest->x=src->x; dest->y=src->y; }

static inline s32 turn_differs_800A6690(Turn_800A6690 *turn, u8 *target) {
    return turn->dir != *target;
}

static inline s32 dir_delta_800A6690(u8 *from, u8 *to) {
    return (*from - *to) & 7;
}

static inline MapPos *Pos_copy_800A67DC(MapPos *dst, MapPos *src) { dst->x = src->x; dst->y = src->y; return dst; }

static inline s32 cell_usable_800A694C(Creature *c, void *center, s32 check) {
    return !(func_800B1C6C(&c->pos) & 0x4000) && !func_800A6E90(c)
        && (!(c->tag & 0x7C) || !func_800E1CC4(c, 1))
        && (!check || func_800A44F4(static_cast<Creature *>(center), c) == 2);
}

static inline s32 Pos_isOut_800A6BA4(MapPos *p) {
    return !(p->y < 0x4C && p->x < 0x36 && p->y >= 0 && p->x >= 0);
}

static inline s32 isBlocked_800A6BA4(MapPos *p, s32 flag) {
    s32 stop = 0;
    if (!(func_800B1C6C(p) & 0xC000) || flag) {
        if (Pos_isOut_800A6BA4(p)) {
            stop = 1;
        }
    } else {
        stop = 1;
    }
    return stop;
}

static inline void Pos_copy_800A7044(MapPos *dst, MapPos *src) {
    dst->x = src->x;
    dst->y = src->y;
}

/* func_800A3D9C */
char *func_800A3D9C(Creature *unit) {
    s32 flag;
    s32 unavailable;
    char *name;
    if (unit->tag & 3) return func_80048480(func_800F4810(unit->kind));
    if (func_800E1CD4(unit, 15)) return func_80048480(0x235);
    flag = 0;
    if (func_800E1CC4(unit, 1)) flag = !D_801476B8->isProtected();
    if (flag) return func_80048480(0x232);
    if (static_cast<LivingVTable *>(unit->vt)->condition90.fn((Creature *)((char *)unit + static_cast<LivingVTable *>(unit->vt)->condition90.delta), 2, 9, 0, 0))
        return func_80048480(0x236);
    unavailable = 0;
    if (func_800E4454(unit)) unavailable = 1;
    else if (((unit->tag >> 4) & 1) && (func_800E0F40(unit) & 0xFF) != static_cast<LivingCreature *>(unit)->field75 && unit->kind != 0x31)
        unavailable = 1;
    if (unavailable) {
        char *buf = D_801429FC;
        char *format = func_80048480(0x233);
        func_800327C0(buf, format, func_800A3B20(unit));
        return buf;
    } else {
        u8 flags = unit->tag;
        name = 0;
        if ((flags >> 3) & 1) name = func_80048480(unit->kind + 0x1F2E);
        else if ((flags >> 5) & 1) {
            s32 kind = unit->kind;
            name = func_80048480(func_800F4044(kind, func_800E0F40(unit)));
        } else if ((flags >> 4) & 1) {
            if (unit->kind == 0x56) {
                char *buf = D_801429FC;
                char *format = func_80048480(0x238);
                func_800327C0(buf, format, func_80048480(static_cast<InventoryCreature *>(unit)->name96));
                return buf;
            }
            name = func_80048480(static_cast<InventoryCreature *>(unit)->name96);
        }
        if (name) {
            char *buf = D_801429FC;
            char *format = func_80048480(0x237);
            func_800327C0(buf, format, func_800E08F0(unit), name);
            return buf;
        }
        return (char *)"";
    }
}

/* func_800A4010 */
s32 func_800A4010(Creature *unit, u16 flags, s32 mode)
{
    s32 special = 0;
    if ((unit->state & 4) || ((unit->tag & 0x7C) && func_800E1CD4(unit, 15))) {
        special = 1;
    }
    if (special) {
        return func_800A4140(flags, mode);
    }
    if (unit->state & 8) {
        return func_800A416C(flags, mode);
    }
    if (unit->state & 0x10) {
        return func_800A4198(flags, mode);
    }
    if (unit->state & 0x20) {
        return func_800A41B4(flags, mode);
    }
    {
        UnitFlags state = unit->flags;
        s32 result;
        if (!Flags_locked_800A4010(&state)) {
            result = func_800A4124(flags, mode);
        } else {
            result = func_800A41D0(flags, mode);
        }
        return result;
    }
}

/* func_800A4124 */
s32 func_800A4124(s32 flags, s32 mode)
{
    if (mode == 2) return (flags & 0xE100) == 0;
    return 0;
}

/* func_800A4140 */
s32 func_800A4140(s32 flags, s32 state) {
    if (flags & 0xC100) return 0;
    if (flags & 0x2000) return state == 1;
    return 1;
}

/* func_800A416C */
s32 func_800A416C(s32 flags, s32 mode)
{
    if (flags & 0xC100) return 0;
    if (flags & 0x2000) return mode == 1;
    return 1;
}

/* func_800A4198 */
s32 func_800A4198(s32 flags, s32 type) {
    if (type == 2) {
        return (flags & 0xC100) == 0;
    }
    return 0;
}

/* func_800A41B4 */
s32 func_800A41B4(s32 arg0, s32 arg1) {
    if (arg1 == 2) {
        return (arg0 & 0xC000) == 0;
    }
    return 0;
}

/* func_800A41D0 */
s32 func_800A41D0(s32 flags, s32 mode) {
    switch (mode) {
    case 2: return (flags & 0x8020) == 0;
    default: return 0;
    }
}

/* func_800A41EC */
s32 func_800A41EC(Creature *arg, MapPos *value) { return func_800A422C(arg, value, func_800A4360(arg, value)); }

/* func_800A422C */
s32 func_800A422C(Creature *o, MapPos *a, s32 b) {
  if ((u8)o->state >> 7) { if (func_800B502C(a)) return 0; }
  if ((o->tag >> 2) & 1) { Item *p = func_800B4D80(a); if (p != 0 && p->field01 == 0xF4) return 0; }
  return func_800A4010(o, func_800B1C6C(a), b); }

/* func_800A42CC */
s32 func_800A42CC(Creature *a, MapPos *b, s32 mode) {
    s32 r = 0;
    if (func_800A422C(a, b, mode)) r = func_800B4888(b) == 0;
    return r;
}

/* func_800A4314 */
s32 func_800A4314(Creature *object, MapPos *position)
{
    s32 kind = func_800A4360(object, position);
    return object->vt->place38.fn((Creature *)((char *)object + object->vt->place38.delta),
                                      position, kind);
}

/* func_800A4360 */
s32 func_800A4360(Creature *obj, MapPos *arg1) {
    s32 found = ((obj->tag & 0x7C) && func_800E1CD4(obj, 0xF) != 0)
        || (!(Obj_status_800A4360(obj) & 0x20) && !(Obj_status_800A4360(obj) & 0x10) && !Obj_locked_800A4360(obj));
    if (found && (func_800B1C6C(arg1) & 0x2000)) {
        return 1;
    }
    return 2;
}

/* func_800A4404 */
s32 func_800A4404(Creature *state, MapPos *position, void **output) {
    MapPos *point;
    s32 direction;
    s32 result;
    *output = 0;
    if (func_800B1C6C(position) & 0x8000) return 0;
    MapPos copy(*position);
    point = &copy;
    direction = state->mode09 & 0xF;
    result = func_800B5900(point, direction, func_800A58B8(state), output);
    if (result == 0 && *output == 0) {
        if (func_800B1C6C(position) & 0x4000) {
            if (func_800A422C(state, position, state->mode09 & 0xF)) return 1;
        }
    }
    return result;
}

/* func_800A44E8 */
s32 func_800A44E8(Creature *obj, Creature *target, u8 *out)
{
    *out = 0;
    return 0;
}

/* func_800A44F4 */
s32 func_800A44F4(Creature *self, Creature *target) {
    u8 result;
    CreatureVTable *vtable = self->vt;
    return vtable->relation40.fn((Creature *)((char *)self + vtable->relation40.delta), target, &result);
}

/* func_800A4520 */
s32 func_800A4520(Creature *ctx, Creature *obj) {
    if (obj != 0 && ((obj->tag >> 1) & 1)) {
        return 1;
    }
    return func_800A44F4(ctx, obj) == 2;
}

/* func_800A455C */
/* The position handed to A282C, A23E8 and A27A4, the flag word and the
 * direction written by A27A4 form one frame record (sp+0x20 MapPos, +0x28 flags,
 * +0x2C MapDir), as in the accepted A455C.c view.  The flag word is stored to and
 * reloaded from the frame after the calls; a separate unaddressed UnitFlags local
 * is kept in a register instead (measured: 63 differing words). */
struct Request_800A455C { MapPos pos; UnitFlags flags; MapDir dir; };
s32 func_800A455C(Creature *obj, Creature *dest, s32 maxSteps) {
    s32 ok;
    s32 i;
    s32 failed;
    if (dest != 0) {
        MapPos cur(obj->pos);
        MapPos target(dest->pos);
        Request_800A455C req;
        ok = 0;
        req.flags = obj->flags;
        req.pos.x = target.x;
        req.pos.y = target.y;
        if (func_800A282C(&cur, &req.pos)) {
            req.pos.x = target.x;
            req.pos.y = target.y;
            if (func_800A23E8(&cur, &req.pos) <= maxSteps) {
                if (func_800A674C(obj, dest) != 0 || req.flags.bits.b22) ok = 1;
            }
        }
        if (ok) {
            req.pos.x = target.x;
            req.pos.y = target.y;
            func_800A27A4(&req.dir, &cur, &req.pos);
            for (i = 0; i < maxSteps; i++) {
                failed = func_800A46BC(obj, &cur, &req.dir) != 1;
                if (failed) return 0;
                func_800A2758(&cur, req.dir);
            }
            return 1;
        }
    }
    return 0;
}

/* func_800A46BC */
s32 func_800A46BC(Creature *obj, MapPos *arg, MapDir *cell) {
    MapPos tmp;
    MapPos *t;

    func_800A2594(&tmp, arg, *cell);
    t = &tmp;
    if (func_800B1C6C(t) & 0x8000) {
        return 0;
    }
    if (((u8)obj->state >> 7) && func_800B502C(t)) {
        return 0;
    }
    return func_800A4754(obj, arg, cell);
}

/* func_800A4754 */
s32 func_800A4754(Creature *self, MapPos *pos, MapDir *dir){
    MapPos tmp;
    s32 left, right, ok, r;
    UnitFlags f;
    func_800A2594(&tmp, pos, Dir_make_800A4754((dir->value + 1) & 7));
    left = func_800B1C6C(&tmp);
    func_800A2594(&tmp, pos, Dir_make_800A4754((dir->value - 1) & 7));
    right = func_800B1C6C(&tmp);
    f = self->flags;
    if (Flags_diagonalOk_800A4754(&f)) {
        if (dir->value & 1) {
            r = 0;
            if (!(left & 0x8020) || (left & 0x200)) {
                if (!(right & 0x8020) || (right & 0x200)) r = 1;
            }
            return r;
        }
    } else {
        func_800A2594(&tmp, pos, Dir_make_800A4754(dir->value));
        ok = 0;
        if (func_800A4360(self, pos) == 1) ok = func_800A5AA0(self) == 1;
        if (ok) {
            MapPos *t = &tmp;
            if (func_800B1C6C(t) & 0x4100) return 0;
            if (dir->value & 1) {
                if (func_800B1C6C(t) & 0x2000) {
                    return terrain_is_water_800A4754(left) && terrain_is_water_800A4754(right);
                }
                return (left & 0x4200) != 0x4000 && (right & 0x4200) != 0x4000;
            }
        } else {
            if (func_800B1C6C(&tmp) & 0x4000) return 0;
            if (dir->value & 1) return (left & 0x4200) != 0x4000 && (right & 0x4200) != 0x4000;
        }
    }
    return 1;
}

/* func_800A492C */
Creature *func_800A492C(Creature *a, s32 b, s32 c, s32 d) { u8 local; return func_800A4954(a,b,&local,c,d); }

/* func_800A4954 */
Creature *func_800A4954(Creature *object, s32 category, u8 *out_priority, s32 use_priority, s32 check_target) {
    UnitIterator iterator;
    Assessment_800A4954 assessment;
    UnitIterator *current;
    MapDir *dir;
    Creature *best = 0;
    s32 best_distance = 76;
    u8 best_priority = 0;
    assessment.priority = 0;
    func_800A915C(&iterator, object);
    current = &iterator;
    dir = &assessment.direction;
    while (func_800A9284(current, 255) != 0) {
        Creature *candidate = static_cast<Creature *>(func_800A942C(current));
        s32 eligible = object->vt->relation40.fn((Creature *)((char *)object + object->vt->relation40.delta), candidate, &assessment.priority) == category &&
            (check_target == 0 || func_800A674C(object, candidate) != 0) &&
            (!(candidate->tag & 124) || func_800E1CC4(candidate, 1) == 0) &&
            func_800A6E90(candidate) == 0 && !(func_800B1C6C(&candidate->pos) & 0x4000);
        if (eligible != 0) {
            s32 distance;
            if (use_priority == 0) {
                assessment.priority = 0;
            }
            distance = func_800A65B8(object, candidate ? &candidate->pos : 0);
            if (distance == 1) {
                func_800A65E4(dir, object, candidate ? &candidate->pos : 0);
                if ((assessment.direction.value ^ 1) & 1) {
                    distance = 0;
                }
            }
            if (assessment.priority > best_priority || (assessment.priority == best_priority && distance < best_distance)) {
                best = candidate;
                best_distance = distance;
                best_priority = assessment.priority;
            }
        }
    }
    *out_priority = best_priority;
    return best;
}

/* func_800A4B24 */
Creature *func_800A4B24(Creature *p, s32 expected, s32 ranked, s32 checked) {
    RayIterator iterator;
    MapPos position;
    unsigned char direction;
    ScanDir scan_direction;
    unsigned char rank = 0;
    Creature *best = 0;
    u32 best_rank;
    s32 remaining;
    s32 best_distance;
    best_distance = 0x4C;
    best_rank = 0;
    remaining = 8;
    direction = p->dir.value;
    for (;;) {
        s32 active = --remaining;
        if (active == -1) break;
        func_800A39C0(&position, p, &direction, 1);
        scan_direction.value = direction;
        func_800C5150(&iterator, &position, scan_direction, 0xFF, 0x4000);
        for (;;) {
            s32 present = iterator_remaining_800A4B24(&iterator);
            Creature *candidate;
            s32 distance;
            CreatureVTable *methods;
            if (!present) break;
            candidate = static_cast<Creature *>(func_800C51B8(&iterator));
            if (checked) {
                s32 result = func_800A674C(p, candidate) ^ 1;
                if (result) continue;
            }
            methods = p->vt;
            if (methods->relation40.fn((Creature *)((char *)p + methods->relation40.delta), candidate, &rank) != expected) break;
            if (ranked && rank < best_rank) break;
            distance = func_800A65B8(p, candidate ? &candidate->pos : 0);
            if (func_800A6420(p, candidate) != 3 && distance < best_distance) {
                best = candidate;
                best_rank = rank;
                best_distance = distance;
            }
        }
        func_800A2F80(&direction, 1);
    }
    return best;
}

/* func_800A4CC4 */
s32 func_800A4CC4(Creature *self, MapPos *pos, MapDir *dir) {
    MapPos next;
    s32 blocked;
    s32 permitted;

    func_800A2594(&next, pos, *dir);
    blocked = 0;
    if (func_800B1C6C(pos) & 0x80) blocked = (self->mode09 & 15) == 1;
    if (blocked) return 0;
    permitted = func_800A46BC(self, pos, dir) == 1;
    if (!permitted) return 0;
    if (self->state & 8) {
        s32 blocked_corner;
        s32 blocked_terrain = !(func_800B1C6C(pos) & 0x2000) || !(func_800B1C6C(&next) & 0x2000);

        if (blocked_terrain) return 0;
        blocked_corner = 0;
        if (dir->value & 1) {
            MapPos left, right;
            MapDir turn, other;

            /* Diagonal step: the right-hand side tile is only looked up when the left one is
             * passable. */
            turn.value = (dir->value + 1) & 7;
            func_800A2594(&left, pos, turn);
            blocked_corner = !(func_800B1C6C(&left) & 0x2000) ||
                (other.value = (dir->value - 1) & 7, func_800A2594(&right, pos, other),
                 !(func_800B1C6C(&right) & 0x2000));
            if (blocked_corner) return 0;
        }
    }
    return func_800A4314(self, &next);
}

/* func_800A4E30 */
s32 func_800A4E30(Creature *position, MapDir *direction) {
    MapPos other;
    Creature *first;
    Creature *second;
    s32 result;
    func_800A2594(&other, &position->pos, *direction);
    first = func_800B49B8(&position->pos);
    second = func_800B49B8(&other);
    result = func_800A4CC4(position, &position->pos, direction);
    func_800B48C0(&position->pos, first);
    func_800B48C0(&other, second);
    return result;
}

/* func_800A4EC0 */
void func_800A4EC0(Creature *state, MapDir *value) {
    func_800A665C(state, value);
    func_800A4F58(state, value);
}

/* func_800A4EFC */
s32 func_800A4EFC(Creature *p, MapDir *a) {
    if (func_800A4CC4(p, func_800A7DEC(p), a)) {
        func_800A4EC0(p, a);
        return 1;
    }
    return 0;
}

/* func_800A4F58 */
void func_800A4F58(Creature *a, MapDir *b) {
    MapPos r;
    if (func_80048AC8()) func_80046B3C(a, *b);
    func_800A2594(&r, &a->pos, *b);
    func_800A592C(a, &r);
}

/* func_800A4FBC */
s32 func_800A4FBC(Creature *a, MapDir *b) {
    if (func_800A4CC4(a, func_800A7DEC(a), b) != 0) {
        func_800A4F58(a, b);
        return 1;
    }
    return 0;
}

/* func_800A5018 */
s32 func_800A5018(Creature *self, s32 kind) {
    MapDir position;
    signed char *direction = D_801429E8[kind & 0xff];
    s32 i = 0;
    position.value = self->dir.value;
    do {
        func_800A2F80(&position.value, *direction);
        if (func_800A4EFC(self, &position)) return 1;
        i++;
        direction++;
    } while (i < 5);
    return 0;
}

/* func_800A50AC */
s32 func_800A50AC(Creature *object) {
    return func_800A5018(object, func_800C57CC(D_80147620, 3) & 0xff);
}

/* func_800A50E8 */
s32 func_800A50E8(Creature *a) {
    MapDir dir;
    MapDir *pdir;
    s32 i;
    dir.value = func_800C57A0(D_80147620) & 7;
    for (i = 0, pdir = &dir; i < 8; i++) {
        if (func_800A4EFC(a, pdir)) return 1;
        func_800A2F80(&pdir->value, 1);
    }
    return 0;
}

/* func_800A5168 */
s32 func_800A5168(Creature *self, MapPos *position, s32 alternate) {
    MapPos old, next;
    MapPos *previous = &old;
    MapPos *dest;
    s32 message;
    old.x = self->pos.x;
    previous->y = self->pos.y;
    if (self->state & 2) {
        func_80049CB4(0x10A8, self);
        func_80049CB4(0x14, self, previous);
        func_80049CB4(0x1F, self);
        return 1;
    }
    next.x = position->x;
    next.y = position->y;
    func_800A59A4(self);
    dest = &next;
    {
        s32 failed = func_800A5D2C(self, dest, 10);
        failed ^= 1;
        if (failed) {
            failed = func_800A60D8(self, dest);
            failed ^= 1;
            if (failed) {
                self->vt->reset18.fn((Creature *)((char *)self + self->vt->reset18.delta));
                return 0;
            }
        }
    }
    assign_position_800A5168(&self->pos, &next);
    message = 0x1C;
    if (alternate) message = 0x1D;
    func_80049CB4(message | 0x1000, self, &old);
    func_800A58FC(self, &next);
    func_800A7BA4(self, 2);
    return 1;
}

/* func_800A529C */
s32 func_800A529C(Creature *obj, s32 flag) {
    MapPos pos;
    s32 ok;

    position_800A529C(&pos, func_800C5F60());
    ok = 0;
    if ((obj->state & 2) || func_800A60D8(obj, &pos)) {
        ok = 1;
    }
    if (ok) {
        return func_800A5168(obj, &pos, flag);
    }
    obj->vt->reset18.fn((Creature *)((char *)obj + obj->vt->reset18.delta));
    return 0;
}

/* func_800A533C */
s32 func_800A533C(Creature *a) {
    s32 failed = func_800A422C(a, &a->pos, a->mode09 & 0xF) != 1;
    if (failed) return func_800A529C(a, 0);
    return 1;
}

/* func_800A5388 */
s32 func_800A5388(Creature *obj, s32 input_mask) {
    UnitFlags fl;
    u16 mask = input_mask;

    if (obj->state & 1) {
        return 1;
    }
    fl = obj->flags;
    if (Flags_bit13_800A5388(&fl) && !(mask & 0x10)) {
        return 2;
    }
    if (mask & 1) {
        return 0;
    }
    if (Flags_bit12_800A5388(&fl) || (mask & 2)) {
        return 1;
    }
    if (!(mask & 4)) {
        return func_800C587C(D_80147620, obj->vt->mode50.fn((Creature *)((char *)obj + obj->vt->mode50.delta), 1)) != 0;
    }
    return 0;
}

/* func_800A5440 */
s32 func_800A5440(Creature *object, Creature *target, u8 *direction, s32 kind, char *item) {
    MapPos position;
    MapPos *point;
    signed char opposite;
    s32 result;
    s32 message;
    s32 blocked;
    s32 hidden;
    result = func_800A5388(object, kind & 0xFFFF);
    hidden = (target != 0 && func_800A6FD0(target) != 0) || func_800A6FD0(object) != 0;
    if (!hidden) {
        point = &position;
        point->x = object->pos.x;
        point->y = object->pos.y;
        message = func_80049CB4(0xDA, point);
    } else {
        message = -1;
    }
    opposite = (*direction + 4) & 7;
    if (result != 1) {
        if (result == 2) {
            if (object->tag & 0x7C) {
                func_80049CB4(0x12F, 4);
                func_800E370C(object, &opposite);
            }
            func_80049CB4(0x66, object);
            D_8013960C = (D_8013960C * 2) | 1;
            func_80049AE8(0x51, message, func_800A3B20(object), item);
            D_8013960C >>= 1;
        }
        return result;
    }
    D_8013960C = (D_8013960C * 2) | 1;
    func_80049CB4(6);
    if (target != 0 && func_800A6FD0(target) != 0) {
        func_80049AE8(0x5A, message, item);
    } else {
        if (object->tag & 0xC) {
            blocked = func_800E2074(object) == 0 || (((object->tag >> 2) & 1) && static_cast<Player *>(object)->controlled104 != 0);
            if (!blocked) {
                if (func_8005DFE8((unsigned char *)item) < 0x34) {
                    func_80049AE8(0x58, message, func_800A3B20(object), item);
                } else {
                    func_80049AE8(0x59, message, func_800A3B20(object), item);
                }
            } else {
                func_80049AE8(0x5B, message, item, func_800A3B20(object));
            }
        } else {
            func_80049AE8(0x5B, message, item, func_800A3B20(object));
        }
    }
    func_80049CB4(7);
    D_8013960C >>= 1;
    func_80049CB4(0x15, object);
    if (object->tag & 0x7C) {
        func_800E370C(object, &opposite);
    }
    return result;
}

/* func_800A56D8 */
void func_800A56D8(Creature *a, MapPos *b, s16 c, u16 d) {
    HitCommand command;
    MoveAction action;
    func_800C4F34(&command, (u16)c);
    d |= func_800A6EE0(a);
    func_800C4DA0(&action, a, &command, d);
    func_800C4864(&action, 0x38, b);
}

/* func_800A5758 */
s32 func_800A5758(Creature *a, MapPos *b, s16 c, u16 d) {
    if (b == 0) {
        return 0;
    }
    func_800A56D8(a, b, c, d);
    return 1;
}

/* func_800A578C */
void func_800A578C(Creature *p) {
    MapPos point; Damage event; UnitFlags snapshot; s32 active = 0;
    if (!(p->state & 1)) { u32 bits = copy_flags_800A578C(&snapshot, &p->flags)->raw; s32 bit = (bits >> 25) & 1; active = bit == 0; }
    if (active) { s16 id; copy_point_800A578C(&point, &p->pos); id = (s16)func_800B5768(&point); if (id && func_800A58B8(p) >= 2) {
        s32 message = func_80049CB4(114, p); Tile_800A578C *tile; Creature *owner;
        if (p->tag & 0x7C) { func_80049CB4(6); func_800497F0(259, message, func_800A3B20(p)); func_80049CB4(7); }
        tile = func_800B51D4(&point); owner = 0; if (tile && tile->field3) owner = D_801476B8;
        create_event_800A578C(&event, owner, id); func_800A7ADC(p, &event);
    } }
}

/* func_800A58B8 */
s32 func_800A58B8(Creature *self) {
    s32 value = self->mode09;
    value &= 15;
    value += func_800A5AA0(self);
    value--;
    if (value >= 3) value = 2;
    return value;
}

/* func_800A58FC */
void func_800A58FC(Creature *actor, MapPos *position) {
    func_800A592C(actor, position);
    func_80049CB4(0x88, actor);
}

/* func_800A592C */
void func_800A592C(Creature *a, MapPos *b) {
    func_800A59A4(a);
    a->pos = *b;
    func_800A5A70(a, func_800A4360(a, b));
    func_800B48C0(b, a);
    func_800A5A04(a);
    func_80049CB4(0xD8, b);
}

/* func_800A59A4 */
void func_800A59A4(Creature *self) {
    MapPos pos(self->pos);
    MapPos *p = &pos;
    if (func_800B4928(p) == self) {
        func_800B49B8(p);
        func_80049CB4(0xD8, p);
    }
}

/* func_800A5A04 */
void func_800A5A04(Creature *object) {
    object->bounds = variant_800A5A04(object)
        ? func_800B3080(&object->pos) : func_800B3024(&object->pos);
}

/* func_800A5A70 */
void func_800A5A70(Creature *object, u32 value) {
    object->mode09 = (object->mode09 & 0xF0U) | (value & 0xFU);
}

/* func_800A5A88 */
void func_800A5A88(Creature *obj, s32 value) {
    obj->mode09 = (obj->mode09 & 0xF) | (value << 4);
}

/* func_800A5AA0 */
u8 func_800A5AA0(Creature *self) {
    return ((self->tag & 0x7C) && func_800E1CD4(self, 15)) ? 1 : (self->mode09 >> 4);
}

/* func_800A5AE8 */
s32 func_800A5AE8(Creature *u, MapPos *pos)
{
    s32 anywhere = D_8014344C == 1;
    s32 tries = 100;
    MapPos candidate;

    for (;;) {
        if (--tries == -1) {
            return 0;
        }
        func_800B221C(&candidate);
        if (!anywhere && func_800B5BDC(&candidate)) {
            continue;
        }
        if (func_800A4314(u, &candidate)) {
            break;
        }
    }
    *pos = candidate;
    return 1;
}

/* func_800A5B98 */
s32 func_800A5B98(Creature *o, MapPos *out) {
    MapPos p;
    s32 i;
    for (i = 0; ; i++) {
        u32 attr;
        if (i >= 100) break;
        func_800B221C(&p);
        if (!canPlace_800A5B98(func_800A4314(o, &p))) continue;
        if (func_800B5BDC(&p)) continue;
        if (o->state & 8) {
            attr = func_800B1C6C(&p);
            if (attr & 0x2000) { *out = p; return 1; }
        } else {
            attr = func_800B1C6C(&p);
            if ((attr & 0xEB70) == 0x200) { *out = p; return 1; }
        }
    }
    for (i = 0; ; i++) {
        Bounds r;
        if (i >= 100) break;
        rect_set_800A5B98(&r, D_801429C0.min.x, D_801429C0.min.y, D_801429C0.max.x, D_801429C0.max.y);
        func_800A33DC(&p, &r);
        if (!canPlace_800A5B98(func_800A4314(o, &p))) continue;
        if (func_800B5BDC(&p)) continue;
        if (o->state & 8) {
            if (func_800B1C6C(&p) & 0x2000) { *out = p; return 1; }
        } else {
            if ((func_800B1C6C(&p) & 0xEB70) == 0x200) { *out = p; return 1; }
        }
    }
    return 0;
}

/* func_800A5D2C */
s32 func_800A5D2C(Creature *object, MapPos *output, s32 flags) {
    RadialIterator iterator;
    MapPos value;
    value.x = output->x;
    value.y = output->y;
    func_800C5280(&iterator, &value, flags);
    for (;;) {
        s32 active = func_800C559C(&iterator);
        if (!active) break;
        func_800C532C(&value, &iterator);
        if (func_800A4314(object, &value)) {
            *output = value;
            return 1;
        }
    }
    return 0;
}

/* func_800A5DC4 */
s32 func_800A5DC4(Creature *self, MapPos *position, s32 flags) {
    s32 result;
    if (!func_800A5D2C(self, position, flags)) {
        CreatureVTable *table = self->vt;
        table->reset18.fn((Creature *)((char *)self + table->reset18.delta));
        result = 0;
    } else {
        func_800A58FC(self, position);
        result = 1;
    }
    return result;
}

/* func_800A5E24 */
s32 func_800A5E24(Creature *self, MapPos *out, s32 sameRoom, s32 occupied) {
    MapPos origin;
    Bounds bounds;
    MapPos candidate;
    Room *room;
    s32 distance;

    copy_coord_800A5E24(&origin, out);
    room = func_800B1F90(&origin);
    distance = 11;
    func_800B1F58(bounds.values);
    for (;;) {
        s32 tries;
        if (--distance < 5) {
            break;
        }
        tries = 20;
        for (;;) {
            s32 blocked;
            u8 refused;
            if (--tries == -1) {
                break;
            }
            pick_coord_800A5E24(self, &bounds, &candidate);
            refused = func_800A4314(self, &candidate) != 1;
            if (refused) continue;
            blocked = (self->state & 8) && !(func_800B1C6C(&candidate) & 0x2000);
            if (blocked) continue;
            if (!sameRoom && in_room_800A5E24(room, &candidate)) continue;
            if (!occupied && is_occupied_800A5E24(&candidate)) continue;
            if (coord_distance_800A5E24(&candidate, &origin) >= distance) {
                *out = candidate;
                return 1;
            }
        }
    }
    return 0;
}

/* func_800A5FCC */
s32 func_800A5FCC(Creature *obj, MapPos *out) {
    Bounds bounds;
    MapPos pos;
    s32 count;
    func_800B1F58(bounds.values);
    count = bounds.values[3] - bounds.values[1] + 1;
    count *= bounds.values[2] - bounds.values[0] + 1;
    func_800A33DC(&pos, &bounds);
    while (--count != -1) {
        if (func_800A4314(obj, &pos)) {
            *out = pos;
            return 1;
        }
        if (++pos.y > bounds.values[3]) {
            pos.y = bounds.values[1];
            pos.x++;
        }
        if (pos.x > bounds.values[2]) {
            pos.y = bounds.values[1];
            pos.x = bounds.values[0];
        }
    }
    return 0;
}

/* func_800A60D8 */
s32 func_800A60D8(Creature *obj, MapPos *out) {
    MapPos pos;
    copy_pos_800A60D8(&pos, &func_800C5F60()->pos);
    if (D_8014344C >= 2 && func_800A5E24(obj, &pos, 0, 1)) {
        *out = pos;
        return 1;
    }
    if (func_800A5E24(obj, &pos, 1, 1)) {
        *out = pos;
        return 1;
    }
    if (func_800A5FCC(obj, &pos)) {
        *out = pos;
        return 1;
    }
    return 0;
}

/* func_800A6184 */
s32 func_800A6184(Creature *self, MapPos *pos) {
    MapPos tmp;
    MapPos *t = &tmp;
    s32 missing;
    t->x = pos->x;
    t->y = pos->y;
    missing = func_800A5E24(self, t, 1, 1) ^ 1;
    if (missing) {
        missing = func_800A5FCC(self, t) ^ 1;
        if (missing) {
            self->vt->reset18.fn((Creature *)((char *)self + self->vt->reset18.delta));
            return 0;
        }
    }
    func_800A58FC(self, &tmp);
    return 1;
}

/* func_800A6218 */
s32 func_800A6218(Creature *self, MapPos *position, s32 notify) {
    MapPos local;
    MapPos *point = copy_point_800A6218(&local, position);
    if (func_800A5D2C(self, point, 1)) {
        s32 old_state, new_state;
        if (notify) { s32 different = func_800A251C(point, position) ^ 1; if (different) func_80049CB4(0x8C, self, position, point); }
        old_state = self->mode09 & 15;
        func_800A58FC(self, &local);
        new_state = self->mode09 & 15;
        if (old_state != new_state && new_state == 1) {
            MapPos *updated = &local;
            if (func_800B1C6C(updated) & 0x2000) func_80049CB4(0x10B, updated);
        }
    } else {
        if (!func_800A60D8(self, point)) {
            self->vt->reset18.fn((Creature *)((char *)self + self->vt->reset18.delta));
            return 0;
        }
        self->pos = *position;
        func_800A5168(self, point, 0);
    }
    return 1;
}

/* func_800A6348 */
s32 func_800A6348(Creature *self, MapPos *pos) {
    MapPos tmp;
    s32 ok = 0;
    MapPos *t = &tmp;
    t->x = pos->x;
    t->y = pos->y;
    if (D_8014344C >= 2) {
        ok = func_800A5E24(self, &tmp, 0, 0) != 0;
    } else if (func_800A5E24(self, &tmp, 1, 1)) {
        ok = 1;
    }
    if (ok) {
        func_800A58FC(self, &tmp);
        return 1;
    }
    if (self != 0) {
        DestroyEntry *e = &self->vt->destroy08;
        e->fn((Creature *)((char *)self + e->delta), 3);
    }
    return 0;
}

/* func_800A63FC */
s32 func_800A63FC(Creature *a, MapPos *b) {
    return func_800A24DC(b, &a->bounds);
}

/* func_800A6420 */
u8 func_800A6420(Creature *obj,Creature *target) {
    MapPos position;
    s32 blocked;
    if(!target) return 3;
    copyPosition_800A6420(&position,&target->pos);
    blocked = !obj->vt->bounds48.fn((Creature *)((char *)obj + obj->vt->bounds48.delta),&position) || ((target->tag & 0x7C) && func_800E1CC4(target,1)) || target->vt->query10.fn((Creature *)((char *)target + target->vt->query10.delta));
    if(blocked) return 3;
    if(func_800A455C(obj,target,1)) return 0;
    if(func_800B20B4(obj,&position)) return 1;
    return 2;
}

/* func_800A650C */
s32 func_800A650C(Creature *origin, MapPos *vector) {
    MapPos copy;
    copy.x = vector->x;
    {
        s32 y = vector->y;
        vector = &copy;
        vector->y = y;
    }
    return func_800A23E8(&origin->pos, vector);
}

/* func_800A6538 */
MapDir *func_800A6538(MapDir *out_direction, Creature *obj, MapPos *target) {
    u8 direction;
    if (func_800A251C(target, &obj->pos)) {
        direction = obj->dir.value;
    } else {
        MapPos copy(*target);
        MapDir toward;
        func_800A27A4(&toward, &obj->pos, &copy);
        direction = toward.value;
    }
    out_direction->value = direction;
    return out_direction;
}

/* func_800A65B8 */
s32 func_800A65B8(Creature *object, MapPos *value) {
    s32 distance;
    if (value) distance = func_800A650C(object, value);
    else distance = 0x4C;
    return distance;
}

/* func_800A65E4 */
MapDir *func_800A65E4(MapDir *p, Creature *q, MapPos *target) {
    if (target == 0) {
        p->value = q->dir.value;
    } else {
        func_800A6538(p, q, target);
    }
    return p;
}

/* func_800A6620 */
MapDir *func_800A6620(MapDir *out, MapPos *from, MapPos *p) {
    MapPos tmp;
    MapPos *t = &tmp;
    t->x = p->x;
    t->y = p->y;
    func_800A27E8(out, from, t);
    return out;
}

/* func_800A665C */
void func_800A665C(Creature *obj, MapDir *value) {
    u8 v = value->value;
    if (v != obj->dir.value) {
        obj->dir.value = v;
        func_80049CB4(0x8B, obj);
    }
}

/* func_800A6690 */
void func_800A6690(Creature *obj, MapDir *target, s32 arg) {
    Turn_800A6690 turn;
    Turn_800A6690 *cur = &turn;

    turn.dir = obj->dir.value;
    while (turn_differs_800A6690(cur, &target->value)) {
        /* ODD_C: promoting the stored byte to s32 preserves the original
         * andi 0xFF followed by the signed slti comparison. */
        s32 delta;
        turn.diff = dir_delta_800A6690(&cur->dir, &target->value);
        delta = turn.diff;
        if (delta >= 5) {
            func_800A2F80(&cur->dir, 1);
        } else {
            func_800A2F94(&cur->dir, 1);
        }
        obj->dir.value = turn.dir;
        func_80049CB4(0x8B, obj);
        func_80049CB4(0x129, arg);
    }
}

/* func_800A674C */
s32 func_800A674C(Creature *self, Creature *other) {
    s32 selfKind;
    s32 selfValue;
    s32 otherKind;

    if (other == 0 || other->vt->query10.fn((Creature *)((char *)other + other->vt->query10.delta))) {
        return 0;
    }
    selfKind = self->mode09 & 0xF;
    selfValue = func_800A58B8(self);
    otherKind = other->mode09 & 0xF;
    return func_800B58E4(selfKind, selfValue, otherKind, func_800A58B8(other));
}

/* func_800A67DC */
s32 func_800A67DC(Creature *u, Creature *o, s32 force, s32 apply) {
    if (o != 0) {
        QueryEntry *e = &o->vt->query10;
        MapPos p;
        MapPos q;
        MapPos t;
        if (e->fn((Creature *)((char *)o + e->delta))) return 0;
        Pos_copy_800A67DC(&p, &o->pos);
        if (force == 0 && (func_800B1C6C(&p) & 0x4000)) return 0;
        if (!func_800A282C(Pos_copy_800A67DC(&q, &u->pos), Pos_copy_800A67DC(&t, &p))) return 0;
        if (apply != 0) {
            unsigned char saved = u->dir.value;
            MapDir d;
            Creature *target;
            func_800A27A4(&d, &q, Pos_copy_800A67DC(&t, &p));
            u->dir.value = d.value;
            target = func_800A6BA4(u, 0x4C, force);
            u->dir.value = saved;
            return func_800A44F4(u, target) == 2;
        }
        return 1;
    }
    return 0;
}

/* func_800A692C */
s32 func_800A692C(Creature *object, s32 code) {
    return func_800B31E8(&object->pos, (unsigned char)code) != 0;
}

/* func_800A694C */
MapPos func_800A694C(void *center, MapRect *bounds, u8 filter, const s32 check) {
    MapPos result(0, 0);
    UnitIterator it;
    s32 count = 0;
    func_800A9204(&it, bounds, center);
    for (;;) {
        Creature *c;
        if (!func_800A9284(&it, filter)) break;
        c = static_cast<Creature *>(func_800A942C(&it));
        if (cell_usable_800A694C(c, center, check)) {
            result = c->pos;
            count++;
        }
    }
    if (count >= 2) {
        count = func_800C57CC(D_80147620, (u8)(count - 1));
        it.cursor = 0;
        for (;;) {
            Creature *c;
            if (!func_800A9284(&it, filter)) break;
            c = static_cast<Creature *>(func_800A942C(&it));
            if (cell_usable_800A694C(c, center, check)) {
                count--;
                if (count == -1) {
                    return c->pos;
                }
            }
        }
    }
    return result;
}

/* func_800A6B70 */
MapPos func_800A6B70(void *source, u8 mode, s32 arg) {
    Creature *query = static_cast<Creature *>(source);
    return func_800A694C(source, &query->bounds, mode, arg);
}

/* func_800A6BA4 */
Creature *func_800A6BA4(Creature *self, s32 count, s32 flag) {
    MapPos pos;
    MapDir dir;
    dir = self->dir;
    pos.x = self->pos.x;
    pos.y = self->pos.y;
    while (1) {
        Creature *e;
        if (count-- <= 0) break;
        func_800A2758(&pos, dir);
        if (isBlocked_800A6BA4(&pos, flag)) return 0;
        e = func_800B4928(&pos);
        if (e == 0) continue;
        if (func_800A674C(self, e)) return e;
    }
    return 0;
}

/* func_800A6CC0 */
MapPos *func_800A6CC0(MapPos *obj, Creature *source) {
    MapDir direction;
    direction.value = source->dir.value;
    func_800A2594(obj, &source->pos, direction);
    return obj;
}

/* func_800A6CF0 */
Creature *func_800A6CF0(Creature *self) { MapPos point; func_800A6CC0(&point, self); return func_800B4928(&point); }

/* func_800A6D18 */
Item *func_800A6D18(Creature *object) {
    MapPos position;
    func_800A6CC0(&position, object);
    return func_800B4D80(&position);
}

/* func_800A6D40 */
Creature *func_800A6D40(Creature *p) {
    MapPos it;
    s32 i;
    Creature *found;
    func_800A2594(&it, &p->pos, p->dir);
    i = 3;
    for (;;) {
        if (i-- <= 0) return 0;
        found = func_800B4928(&it);
        if (found) return found;
        if ((func_800B1DF8(&it) ^ 1) != 0) break;
        func_800A2758(&it, p->dir);
    }
    return 0;
}

/* func_800A6DC8 */
Item *func_800A6DC8(Creature *u) { return func_800B4D80(&u->pos); }

/* func_800A6DE4 */
Inventory *func_800A6DE4(Creature *obj) {
    u8 flags = obj->tag;
    if (flags & 0xC) {
        InventoryVTable *table = static_cast<InventoryVTable *>(obj->vt);
        return table->inventory98.fn((Creature *)((u8 *)obj + table->inventory98.delta));
    }
    if ((flags >> 4) & 1) {
        return static_cast<InventoryCreature *>(obj)->inventory8C;
    }
    if (obj->kind == 0x5A) {
        return &static_cast<InlineInventoryCreature *>(obj)->inventory8C;
    }
    return 0;
}

/* func_800A6E50 */
int func_800A6E50(Creature *arg0,
                  ItemInfo *arg1) {
    int result;

    if (!((arg0->tag >> 2) & 1)) {
        result = arg1->field02 & 0x10;
        result = result == 0;
    } else {
        result = func_800EC630(arg0, arg1);
    }
    return result;
}

/* func_800A6E90 */
s32 func_800A6E90(Creature *p) {
    s32 r = 0;
    if (func_800B1C6C(&p->pos) & 0x80) r = func_800A58B8(p) == 1;
    return r;
}

/* func_800A6EE0 */
/* Event flags of a unit: bits 17/18 of its flag word as a two-bit value, plus 0x30 when
   flag bit 0 is set or the unit answers query 2 (func_800A692C). */
s32 func_800A6EE0(Creature *obj) {
    UnitFlags f;
    unsigned short result;
    /* ODD_C: the do/while (0) groups taking the flag snapshot and decoding bits 17/18; the
       break is taken whenever bit 18 is clear and skips the |2. The loop scope also shapes
       register allocation: its loop notes weight the block's register references, so the
       bit-17 value gets v0 and the copied word v1. Measured: the same statements without
       the block differ by 8 words (v0/v1 swapped). */
    do {
        f = obj->flags;
        result = f.bit17();
        if (!f.bit18()) break;
        result |= 2;
    } while (0);
    if (f.test(1) || func_800A692C(obj, 2)) result |= 0x30;
    return result;
}

/* func_800A6F58 */
s32 func_800A6F58(Creature *self, s32 mode) {
    switch (mode) {
    case 4:
        return D_8015698C;
    case 1:
        return 100 - D_8015691C;
    }
    return 0;
}

/* func_800A6F98 */
s32 func_800A6F98(Creature *object, s32 mode) { return func_800C587C(D_80147620, (unsigned char)object->vt->mode50.fn((Creature *)((char *)object + object->vt->mode50.delta), mode)); }

/* func_800A6FD0 */
s32 func_800A6FD0(Creature *self) {
    s32 r = 0;
    if ((self->tag >> 2) & 1 || func_800C5F60() == self) r = 1;
    return r;
}

/* func_800A7024 */
s32 func_800A7024(Creature *object) {
    return func_801F258C(object->kind, 1);
}

/* func_800A7044 */
MapPos *func_800A7044(MapPos *out, Creature *unit, MapDir *dir, s32 range, s32 flag) {
    MapPos pos;
    MapPos start;
    MapPos cur;
    PathIterator iter;
    void *hit;
    s32 blocked;
    s32 stop;

    Pos_copy_800A7044(&pos, &unit->pos);
    blocked = (unit->state & 2) || ((func_800B1C6C(&pos) & 0x80) && (unit->mode09 & 0xF) == 1);
    if (blocked) {
        Pos_copy_800A7044(out, &pos);
    } else {
    Pos_copy_800A7044(&start, &pos);
    func_800A2594(&cur, &pos, *dir);
    func_800C25D0(&iter, &cur, &dir->value, range);
    while (1) {
        if (iter.index >= iter.length) break;
        func_800C2758(&cur, &iter);
        hit = 0;
        stop = !func_800A4754(unit, &start, dir) || !func_800A4404(unit, &cur, &hit)
            || (flag && (func_800B1C6C(&cur) & 0x2000));
        if (stop) {
            if (hit == 0 || ((static_cast<Creature *>(hit)->state & 1) ^ 1) == 0) {
                MapDir back;
                back.value = (dir->value + 4) & 7;
                func_800A2758(&cur, back);
            }
            break;
        }
    }
    Pos_copy_800A7044(out, &cur);
    }
    return out;
}

/* func_800A7204 */
void func_800A7204(Creature *u, Creature *attacker, MapDir *facing, s32 range, s32 damage, s32 kind, s32 checkWall, s32 notify) {
    if (u->state & 1) {
        return;
    }
    s32 immobile = 0;
    if ((u->state & 2) || ((func_800B1C6C(&u->pos) & 0x80) && (u->mode09 & 0xF) == 1)) {
        immobile = 1;
    }
    if (immobile) {
        if (damage) {
            func_800A7B18(u, attacker, damage, kind);
        }
        return;
    }
    Creature *target = 0;
    MapPos start(u->pos);
    s32 bounced = 0;
    MapDir face(*facing);
    MapPos cur(start);
    MapPos next(start);
    s32 origKind = u->mode09 & 0xF;
    s32 sound = kind == 7 ? 0x18 : 0x17;
    func_800A59A4(u);
    MapDir back(face.turned(4));
    func_800A665C(u, &back);
    func_80049CB4(0x1131);
    s32 moved = 0;
    s32 steps = range;
    while (--steps != -1) {
        func_800A2758(&next, face);
        s32 blocked = 0;
        void *hit = 0;
        if (!func_800A4754(u, &cur, &face)) {
            blocked = 1;
        } else if (!func_800A4404(u, &next, &hit)) {
            blocked = 1;
        } else if (checkWall && (func_800B1C6C(&next) & 0x2000)) {
            blocked = 1;
        }
        if (blocked) {
            if (hit != 0) {
                UnitFlags hitFlags(static_cast<Creature *>(hit)->flags);
                if (!bounced && hitFlags.canReflect()) {
                    face = face.turned(4);
                    func_800A2758(&next, face);
                    bounced = 1;
                    func_80049CB4(sound | 0x1000, u, &start, &next);
                    func_800A665C(static_cast<Creature *>(hit), &face);
                    func_80049CB4(0x1066, hit);
                    start = next;
                    cur = next;
                    continue;
                }
                target = static_cast<Creature *>(hit);
            } else {
                func_800A2758(&next, face.turned(4));
            }
            break;
        }
        if (func_800B56F0(&next)) {
            func_80049CB4(sound, u, &start, &next);
            u->pos = next;
            func_80049CB4(6);
            s32 name = func_80049CB4(0x116, &next);
            func_80049CB4(7);
            if (!moved) {
                moved = 1;
                UnitFlags moveFlags(u->flags);
                if (!moveFlags.isSilent()) {
                    func_80049CB4(6);
                    func_800497F0(0x103, name, func_800A3B20(u));
                    func_80049CB4(7);
                }
            }
            Damage fx(attacker, func_800B5768(&next));
            func_80049CB4(6);
            func_800A7ADC(u, &fx);
            func_80049CB4(7);
            if (u->isGone()) {
                return;
            }
            start = next;
        }
        cur = next;
    }
    func_80049CB4(sound | 0x1000, u, &start, &next);
    u->pos = next;
    if (target) {
        u16 before = 0;
        if (u->tag & 0x7C) {
            before = func_800E08B0(u);
        }
        func_800A0600();
        target->state |= 0x100;
        func_800A7B18(target, attacker, 5, 0x26);
        target->state &= ~0x100;
        func_800A0618(u);
        u16 after = (u->tag & 0x7C) ? func_800E08B0(u) : 0;
        if (after < before) {
            damage = 0;
        }
        if (u->isGone()) {
            return;
        }
    }
    if (func_800A4314(u, &next)) {
        func_800A58FC(u, &next);
    } else {
        s32 i;
        MapPos step;
        face = face.turned(4);
        start = next;
        for (i = 7; i != -1; i--) {
            func_800A2594(&step, &start, face);
            next = step;
            if (func_800A4314(u, &next)) {
                func_80049CB4(0x8C, u, &start, &next);
                func_800A58FC(u, &next);
                break;
            }
            func_800A2F94(&face.value, 1);
        }
        if (i < 0) {
            if (!func_800A60D8(u, &start)) {
                u->vanish();
                return;
            }
            func_800A5168(u, &start, 0);
        }
    }
    if (damage) {
        func_800A7B18(u, attacker, damage, kind);
        if (u->isGone()) {
            return;
        }
    }
    s32 nowKind = u->mode09 & 0xF;
    next = u->pos;
    if (origKind != nowKind && nowKind == 1 && (func_800B1C6C(&next) & 0x2000)) {
        func_80049CB4(0x10B, &next);
    }
    func_80049CB4(0x1F, u);
    if (notify) {
        func_800A7BA4(u, 1);
    }
}

/* func_800A7824 */
s32 func_800A7824(Creature *unit, u8 steps) {
    MapPos start;
    MapPos cur;
    MapPos *next;
    s32 moved;
    s32 stuck = (unit->state & 0x40) || (unit->state & 1);
    if (stuck) {
        return 0;
    }
    start.x = unit->pos.x;
    start.y = unit->pos.y;
    moved = 0;
    {
        /* The byte countdown terminates when decrementing zero wraps to 0xFF. */
        s32 stop = 0xFF;
        cur.x = unit->pos.x;
        cur.y = unit->pos.y;
        next = &cur;
        while (1) {
            s32 blocked;

            if (--steps == stop) {
                break;
            }
            blocked = !(func_800B1C6C(&cur) & 0x80) || !func_800A4CC4(unit, next, &unit->dir) ||
                func_800B4F74(next) || func_800B56F0(next);
            if (blocked) {
                break;
            }
            func_800A2758(&cur, unit->dir);
            moved++;
        }
    }
    if (func_80048AC8()) {
        steps = moved;
        while (steps-- != 0) {
            func_80046B3C(unit, unit->dir);
            func_800A2758(&unit->pos, unit->dir);
            if (func_800A6FD0(unit)) {
                func_800E2298(unit);
            }
            func_80049CB4(0xD7, unit);
        }
    }
    if (moved > 0) {
        unit->pos = start;
        func_800A59A4(unit);
        unit->pos = cur;
        func_800A5A70(unit, func_800A4360(unit, &cur));
        func_800B48C0(&cur, unit);
    }
    return moved;
}

/* func_800A7A1C */
s32 func_800A7A1C(Creature *place) {
    Item *target = func_800B4D80(&place->pos);
    if (target == 0) return 0;
    if (target->field00 != 0x10) return 0;
    TrapMessage msg;
    ItemMethods *vt;
    TrapMessage *pmsg;
    msg.base.kind = 0x15;
    msg.pos = place->pos;
    pmsg = &msg;
    pmsg->direction = place->dir;
    vt = target->field08;
    vt->handle3C((u8 *)target + vt->delta38, &pmsg->base);
    return 1;
}

/* func_800A7A9C */
void func_800A7A9C(Creature *obj, Damage *message, Outcome *result) {
    ResultMessage event;
    ResultMessage *ev = &event;
    event.kind = 8;
    ev->payload.damage = message;
    ev->result = result;
    obj->vt->message58.fn((Creature *)((u8 *)obj + obj->vt->message58.delta), ev);
}

/* func_800A7ADC */
void func_800A7ADC(Creature *entity, Damage *damage) {
    CreatureMessage message;
    CreatureMessage *msg = &message;
    MessageEntry *entry;
    msg->kind = 9;
    msg->payload.damage = damage;
    entry = &entity->vt->message58;
    entry->fn((Creature *)((u8 *)entity + entry->delta), msg);
}

/* func_800A7B18 */
void func_800A7B18(Creature *target, Creature *source, s32 amount, s32 kind) {
    Damage buf;
    Damage *p = &buf;
    func_80136910(p, source, (s16)amount, kind, 8);
    func_800A7ADC(target, p);
}

/* func_800A7B68 */
void func_800A7B68(Creature *object, Damage *damage) {
    CreatureMessage message;
    CreatureMessage *payload = &message;
    CreatureVTable *table;
    message.kind = 10;
    payload->payload.damage = damage;
    table = object->vt;
    table->message58.fn((Creature *)((u8 *)object + table->message58.delta), &message);
}

/* func_800A7BA4 */
void func_800A7BA4(Creature *obj, s32 value) {
    CreatureMessage message;
    CreatureMessage *p = &message;
    message.kind = 0x14;
    p->payload.value = (u8)value;
    obj->vt->message58.fn((Creature *)((u8 *)obj + obj->vt->message58.delta), p);
}

/* func_800A7BE4 */
s32 func_800A7BE4(Creature *arg, Creature *source) {
    CreatureMessage message;
    message.kind = 0x11;
    message.source = source;
    return arg->vt->message58.fn((Creature *)((char *)arg + arg->vt->message58.delta), &message);
}

/* func_800A7C1C */
void func_800A7C1C(Creature *obj)
{
    func_80049CB4(0x88, obj);
    func_80049CB4(0x9D, obj);
}

/* func_800A7C54 */
void func_800A7C54(Creature *self, SaveStream *obj) {
    SaveStreamVTable *vt;
    func_800CA4A4(obj, D_801535DC);
    vt = obj->vt;
    vt->write18.fn((SaveStream *)((u8 *)obj + vt->write18.delta), 0xC, self);
}

/* func_800A7CAC */
void func_800A7CAC(Creature *arg0, SaveStream *obj) {
    SaveStreamEntry *entry;
    func_800CA4E8(obj, D_801535DC);
    entry = &obj->vt->read28;
    entry->fn((SaveStream *)((u8 *)obj + entry->delta), 0xC, arg0);
}

/* func_800A7D04 */
s32 func_800A7D04(Creature *object, s32 mask) { return (object->tag & mask) != 0; }

/* func_800A7D14 */
s32 func_800A7D14(Creature *arg0) { return arg0->tag & 1; }

/* func_800A7D20 */
s32 func_800A7D20(Creature *object) { return (object->tag >> 1) & 1; }

/* func_800A7D30 */
s32 func_800A7D30(Creature *p) { return (p->tag >> 5) & 1; }

/* func_800A7D40 */
s32 func_800A7D40(Creature *self) { return (self->tag >> 4) & 1; }

/* func_800A7D50 */
s32 func_800A7D50(Creature *object) { return (object->tag >> 6) & 1; }

/* func_800A7D60 */
s32 func_800A7D60(Creature *obj)
{
    return (obj->tag & 0xC) != 0;
}

/* func_800A7D70 */
s32 func_800A7D70(Creature *object)
{
    return (object->tag >> 3) & 1;
}

/* func_800A7D80 */
s32 func_800A7D80(Creature *item) { return func_800A3A30(item->kind); }

/* func_800A7D9C */
s32 func_800A7D9C(Creature *obj) { return (obj->tag >> 2) & 1; }

/* func_800A7DAC */
s32 func_800A7DAC(Creature *item) { return (item->tag & 0x7C) != 0; }

/* func_800A7DBC */
s32 func_800A7DBC(Creature *object) { return (object->tag & 3) != 0; }

/* func_800A7DCC */
u8 func_800A7DCC(Creature *obj)
{
    return obj->baseKind;
}

/* func_800A7DD4 */
unsigned char func_800A7DD4(Creature *record)
{
    return record->kind;
}

/* func_800A7DDC */
MapRect *func_800A7DDC(Creature *obj) { return &obj->bounds; }

/* func_800A7DE4 */
MapDir *func_800A7DE4(Creature *obj) { return &obj->dir; }

/* func_800A7DEC */
MapPos *func_800A7DEC(Creature *value) { return &value->pos; }

/* func_800A7DF4 */
s32 func_800A7DF4(s32 arg0, s32 arg1) {
    return arg1;
}

/* func_800A7DFC */
UnitFlags *func_800A7DFC(UnitFlags *result, Creature *object) { *result = object->flags; return result; }

/* func_800A7E0C */
void func_800A7E0C(Creature *object, MapPos *target, s32 mode) {
    MapDir direction;
    MapDir *pair_ptr = &direction;
    func_800A65E4(pair_ptr, object, target);
    func_800A6690(object, pair_ptr, mode);
}

/* func_800A7E68 */
void func_800A7E68(Creature *object, MapPos *target_position, s32 mode) {
    MapDir point;
    MapDir *temporary = &point;
    func_800A6538(temporary, object, target_position);
    func_800A6690(object, temporary, mode);
}

/* func_800A7EC4 */
void func_800A7EC4(Creature *p, MapPos *target) { MapDir t; MapDir *tp = &t; func_800A65E4(tp, p, target); func_800A665C(p, tp); }

/* func_800A7F0C */
void func_800A7F0C(Creature *self, MapPos *target) {
    MapDir tmp;
    MapDir *t = &tmp;
    func_800A6538(t, self, target);
    func_800A665C(self, t);
}

/* func_800A7F54 */
void func_800A7F54(Creature *obj) {
    func_800A4EFC(obj, func_800A7DE4(obj));
}

/* func_800A7F84 */
s32 func_800A7F84(Creature *obj) {
    MapDir *arg = func_800A7DE4(obj);
    return func_800A4CC4(obj, func_800A7DEC(obj), arg);
}

/* func_800A7FCC */
s32 func_800A7FCC(Creature *p, MapDir *dir) { return func_800A4CC4(p, func_800A7DEC(p), dir); }

/* func_800A800C */
s32 func_800A800C(Creature *arg, MapDir *dir) { return func_800A46BC(arg, func_800A7DEC(arg), dir); }

/* func_800A804C */
s32 func_800A804C(Creature *obj) {
    return obj->mode09 & 0xF;
}

/* func_800A8058 */
void func_800A8058(CopyRecord8058 *arg0, CopyRecord8058 *arg1) { *arg0 = *arg1; }

/* func_800A806C */
void func_800A806C(Creature *p, s32 v) { p->dir.value = v >> 24; }

/* func_800A8078 */
void func_800A8078(Creature *state, u32 mask) { state->flags.raw &= ~mask; }

/* func_800A808C */
void func_800A808C(Creature *p, s32 mask) { p->flags.raw &= ~mask; }

/* func_800A80A0 */
void func_800A80A0(Creature *object, u32 mask)
{
    object->flags.raw |= mask;
}

/* func_800A80B0 */
void func_800A80B0(Creature *record, u32 flags)
{
    record->flags.raw |= flags;
}

/* func_800A80C0 */
void func_800A80C0(Creature *obj, u32 value)
{
    obj->flags.raw = value;
}

/* func_800A80C8 */
void func_800A80C8(Creature *record, u32 value)
{
    record->flags.raw = value;
}

/* func_800A80D0 */
void func_800A80D0(Creature *obj)
{
    obj->flags.raw = 0;
}

/* func_800A80D8 */
void func_800A80D8(Creature *obj, s32 value) {
    obj->baseKind = value;
}

/* func_800A80E0 */
void func_800A80E0(Creature *obj, s32 packed) { obj->dir.value = packed >> 24; }

/* func_800A80EC */
void func_800A80EC(CopyRecord80EC *destination, const CopyRecord80EC *source) {
    *destination = *source;
}

/* func_800A8100 */
void func_800A8100(Creature *obj, s32 value) {
    obj->tag = value;
}

/* func_800A8108 */
void func_800A8108(Creature *a, u8 v) { a->kind = v; a->baseKind = v; }

/* func_800A8114 */
char *func_800A8114(Creature *self, char *buffer) {
    return "";
}

}
