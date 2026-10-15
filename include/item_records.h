/* Shared item / item-pool / pool-handle / cursor contracts (wave25 item-name closure).
 * Every translation unit that declares, defines or uses a function or object declared here
 * includes this header, so the compiler checks each definition and use against these exact
 * declarations. Each field cites its producer or consumer evidence; unmodelled bytes are
 * opaque partial views. Scalar types are spelled out because C units typedef u8/u16. */
#ifndef ITEM_RECORDS_H
#define ITEM_RECORDS_H
#include "common.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Item Item;
typedef struct ItemPool ItemPool;

/* Message received by an item's handler slot (+0x38). Its first word is the message kind;
 * senders append kind-specific words (func_800AFEB4 and func_800DE850 send 0x1C,
 * func_800FF828 0x18), so each sender embeds this base as its first member. */
typedef struct ItemMessage {
    s32 kind;
} ItemMessage;

/* Item method table (g++ 2.x vtable entries {delta, index, function}); the item base
 * constructor func_800AC0C0 installs D_80153AA0. Modelled slots: +0x08 deleting destructor
 * (self, flags) called with 3 by func_800AC5F4, func_800B0348/0438, func_800D4AE8,
 * func_800D5160/5514, func_80104D84 and the interpreter (801EE650..801EE658); +0x28 load
 * (self, stream) called by func_800B0164; +0x38 message handler called by func_800AFEB4,
 * func_800DE850 and func_800FF828. +0x10/+0x18 are the tests used by func_800AE498 /
 * func_800AC9D8; +0x30 is the name formatter used by func_800AE674. These are partial
 * table views: unused placeholder slots can point to func_800D8F80 (void no-op);
 * no compatible callable formatter is claimed for such placeholders. */
typedef struct ItemMethods {
    unsigned char pad00[8];
    short delta08;
    short index0A;
    void (*method0C)(void *self, s32 flags);
    short delta10;
    short index12;
    s32 (*test14)(void *self);
    short delta18;
    short index1A;
    s32 (*test1C)(void *self, s32 query);
    unsigned char pad20[8];
    short delta28;
    short index2A;
    void (*load2C)(void *self, void *stream);
    short delta30;
    short index32;
    char *(*format34)(Item *self, char *destination);
    short delta38;
    short index3A;
    s32 (*handle3C)(void *self, ItemMessage *message);
} ItemMethods;

/* Kind 7 table D_8015D5E0, installed by func_80111530. Observed targets:
 * +0x44 func_80112434, +0x4C func_80111D80, +0x5C func_801123EC.
 * The five-argument +0x54 call in func_80111B5C is preserved; the base table's
 * no-op target func_800D8F80 has a pre-existing incompatible prototype, not a fix. */
typedef struct ItemChargesMethods {
    ItemMethods base;
    short delta40, index42;
    s32 (*test44)(void *self);
    short delta48, index4A;
    void (*use4C)(void *self, void *unit);
    short delta50, index52;
    void (*apply54)(void *self, void *actor, void *target, void *context, void *source);
    short delta58, index5A;
    s32 (*query5C)(void *self, void *actor, Item *item);
} ItemChargesMethods;

/* Kind 9 table D_8015D938 (func_80114060/140C0): +0x44 is func_80115540.
 * Downcasts from the initial ItemMethods member require this constructor family. */
typedef struct ItemPotMethods {
    ItemMethods base;
    short delta40, index42;
    Item *(*apply44)(void *self, void *actor, Item *item);
} ItemPotMethods;

/* Item-list method table (list family D_80154300; the pot content list uses D_80154550,
 * whose ROM words put func_800CEA78 at +0x14 and func_800CE710 at +0x24). Slot +0x10: the
 * list's limit byte +0xD (func_800CEA78; called by func_800F27A4). Slot +0x20: the entry
 * count byte +0xE (func_800CE710; called by func_80104D84 and func_800F0440).
 * +0x18 sets the limit (func_800CE6F8), +0x38 gets an item (func_800CE7A0),
 * +0x60 checks source-list membership and whether release is allowed (func_800CFFC8).
 * Full table extents are not claimed. */
typedef struct ItemListMethods {
    unsigned char pad00[0x10];
    short delta10;
    short index12;
    s32 (*limit14)(void *self);
    short delta18;
    short index1A;
    void (*set1C)(void *self, s32 value);
    short delta20;
    short index22;
    s32 (*count24)(void *self);
    unsigned char pad28[0x10];
    short delta38;
    short index3A;
    Item *(*get3C)(void *self, u32 index);
    unsigned char pad40[0x20];
    short delta60;
    short index62;
    s32 (*can_release64)(void *self, Item *item, s32 notify);
} ItemListMethods;

/* The 0x10-byte list prefix constructed by func_800CE620/CE658. Inline entries
 * belong to the containing pot, not to every generic list. */
typedef struct ItemList {
    ItemPool *pool;
    const ItemListMethods *methods;
    unsigned char *entries;
    unsigned char state[4];
} ItemList;

/* Kinds 3 and 4: their constructors func_8010EA20 / func_8010CC70 call func_8010B8D0(self,
 * 3 or 4, id) and then func_8010EAA8 / func_8010CCD0, which store +0xC and call func_8010BC98
 * to store the capacity +0xE capped at 16. Signed +0xC/+0xD (func_800A0EF4 reads them
 * signed), count +0xF and sixteen entries from +0x10 (func_8010BD00 appends, func_8010BE60
 * removes and reads the count with lbu). Kind 3 alone additionally owns +0x20/+0x21:
 * func_8010EA20 initializes them; func_8010EDD4 decrements +0x20. Discriminants:
 * func_800ECF38 and func_800FF828 use func_800E8A68(unit, 3 or 4), whose search requires kind
 * byte == 3 or 4; func_80104D84 switches on kind 3/4. func_800A1218 has no kind test (its
 * caller's cursor selection is a precondition). */
typedef struct ItemSeals {
    signed char field0C;
    signed char field0D;
    unsigned char capacity0E;
    unsigned char count0F;
    unsigned char contents10[16];
    unsigned char field20;
    unsigned char field21;
} ItemSeals;

/* Kind 9 (constructors func_80114060 and func_801140C0 call func_800AC0C0(self, 9, id)):
 * func_800CFF00 builds the content list at +0xC through func_800CE620/CE658 (pool
 * &D_80143094 at +0xC, list method table D_80154550 at +0x10, entry pointer +0x14 to the
 * eight inline entries at +0x1C, capacity/limit/count bytes +0x18..+0x1A) and stores the
 * owning pot at +0x24; both constructors then store +0x29 = 0 and +0x28 = 1.
 * func_801216D0 (id 0xAC) stores +0x2C = 0xFF after func_801140C0. Consumers test kind 9
 * (func_800B0438, func_800ECF38 stores +0x28 = 1, func_80104D84) and id 0xAC before +0x2C
 * (func_80104D84 stores 0xFF). */
typedef struct ItemPot {
    ItemList list;
    unsigned char entries[8];
    Item *owner;
    unsigned char field28;
    unsigned char field29;
    unsigned char pad2A[2];
    unsigned char field2C;
} ItemPot;

/* Kind 0x13, id 0xF2: func_80128C40 calls func_80117230 (the kind-0x13
 * constructor); func_80128CAC reads both bytes and func_80128EE8 sets flags0D. */
typedef struct ItemAppearance {
    unsigned char field0C;
    unsigned char flags0D;
} ItemAppearance;

/* Kind 0x13, id 0xF1: func_80128280 calls func_80117230(self, 0xF1),
 * clears +0xC..+0xF and stores the count byte +0x10 = 1. */
typedef struct ItemContainerF1 {
    unsigned char field0C;
    unsigned char field0D;
    unsigned char field0E;
    unsigned char field0F;
    unsigned char count10;
} ItemContainerF1;

typedef struct ItemPosition { s32 x; s32 y; } ItemPosition;

/* Kind 2: func_80112D20 initializes the unsigned flag byte +0xC. Its id-0x24 and
 * id-0x27 constructors (func_8011B930/func_8011BF90) initialize +0x10/+0x14. */
typedef struct ItemConsumable {
    unsigned char flags0C;
    unsigned char pad0D[3];
    ItemPosition position10;
} ItemConsumable;

/* Kind 0x10: func_80115690 initializes the unsigned flag byte +0xC. */
typedef struct ItemTrap {
    unsigned char flags0C;
} ItemTrap;

/* Kind 6: func_80113520 initializes +0xC/+0xD; func_801137CC and
 * func_80113C68 consume them unsigned. */
typedef struct ItemTimer {
    unsigned char turns0C;
    unsigned char flags0D;
} ItemTimer;

/* Kind 7: func_80111530 initializes used0D and calls func_80111608 for
 * count0C. Only the id-0x98/0x99 subclass (func_8011DA10 via func_8011F2B0 /
 * func_8011F310) owns child10 and childEnabled11. */
typedef struct ItemCharges {
    unsigned char count0C;
    unsigned char used0D;
    unsigned char pad0E[2];
    unsigned char child10;
    unsigned char childEnabled11;
} ItemCharges;

/* Kind 0x11: func_801128F0 and its derived constructors install base state and tables,
 * not level0C. func_800AB35C case 17 calls func_800AB750 to set the signed level.
 * The id-0xE9 subclass reads its low byte (+0xF, big endian). */
typedef union ItemLevel {
    s32 level0C;
    unsigned char bytes0C[4];
} ItemLevel;

/* Kind 0xE (func_8010E020): the same +0xC word is consumed unsigned by
 * func_800F9564 / func_800EC384, signed by func_8010E05C. */
typedef union ItemAmount {
    u32 amount0C;
    s32 signed0C;
} ItemAmount;

/* Kind 0x12 (func_80117080): signed +0xC is consumed by func_801170F4. */
typedef struct ItemValue { s32 value0C; } ItemValue;

/* One 0x30-byte item record. func_800AC5B4 IGNORES the requested constructor
 * size and returns a full slot of pool D_80143094; func_800AC5F4 recycles existing
 * storage. The only traced non-pool placements (func_800923F0 / func_8009E724)
 * explicitly provide 0x30 bytes. Thus 0x10/0xC SIZE ARGUMENTS ARE NOT ALLOCATIONS.
 * func_800AFD78 proves the 0x30 stride; pool copy paths preserve all record bytes.
 * The first 12 bytes are the constructor-established common prefix; each payload
 * member requires its documented kind/id or preserved raw-record interpretation.
 * In particular func_800514F0 may use an id-0xF2 displayed category to select a
 * different payload interpretation; that is NOT a claim of a changed actual kind. */
struct Item {
    unsigned char field00;
    unsigned char field01;
    unsigned char flags02;
    unsigned char field03;
    unsigned char field04;
    signed char owner05;
    unsigned char pad06[2];
    ItemMethods *field08;
    union {
        ItemSeals seals;
        ItemPot pot;
        ItemAppearance appearance;
        ItemContainerF1 containerF1;
        ItemConsumable consumable;
        ItemTrap trap;
        ItemTimer timer;
        ItemCharges charges;
        ItemLevel level;
        ItemAmount amount;
        ItemValue value;
    } payload;
};


/* The item-family name formatter uses the table at +8, signed receiver delta +0x30,
 * and char* formatter slot +0x34. The destination rotates through four 32-byte
 * buffers. Only the constructed base prefix is read, not the entire pool record. */
char *func_800AE674(Item *item);
/* Concrete +0x34 targets from constructor-proven item tables. All 157 observed
 * table entries have delta30/index32 zero. Proved source-visible receiver roots
 * use 48-byte pool/stack records; assembly-only receiver provenance remains held.
 * Preserve the signed receiver adjustment. Placeholder func_800D8F80 is not a formatter. */
char *func_8010B9FC(Item *self, char *destination);
char *func_8010DDB0(Item *self, char *destination);
char *func_8010DE6C(Item *self, char *destination);
char *func_8010E328(Item *self, char *destination);
char *func_8010E6A0(Item *self, char *destination);
char *func_8011186C(Item *self, char *destination);
char *func_80112608(Item *self, char *destination);
char *func_80112AF8(Item *self, char *destination);
char *func_80113100(Item *self, char *destination);
char *func_80113564(Item *self, char *destination);
char *func_80114D34(Item *self, char *destination);
char *func_801155AC(Item *self, char *destination);
char *func_8011637C(Item *self, char *destination);
char *func_80116C6C(Item *self, char *destination);
char *func_80116D8C(Item *self, char *destination);
char *func_801170F4(Item *self, char *destination);
char *func_8011726C(Item *self, char *destination);
char *func_8011DBB0(Item *self, char *destination);
char *func_80121F58(Item *self, char *destination);
char *func_801230B4(Item *self, char *destination);
char *func_801239FC(Item *self, char *destination);
char *func_80123AEC(Item *self, char *destination);
char *func_80127C50(Item *self, char *destination);
char *func_80128560(Item *self, char *destination);
char *func_80128CAC(Item *self, char *destination);

/* Item pool header (0x10 bytes; D_80143094.. are spaced 0x10 apart), built by
 * func_800AF890: record storage, occupancy bits (cleared by func_800AF8D0, tested by
 * func_800AF920, set/cleared by func_800AFCD8/AFCA8), the capacity that func_800AFD78
 * bounds indexes with (lw +8), and the end of the primary region: func_800AFA7C allocates
 * in [0, primaryCount), the alternate allocator func_800AFAFC in [primaryCount, capacity),
 * and func_800AF910 sets primaryCount to capacity minus the reserved count. */
struct ItemPool {
    Item *items;
    unsigned char *occupancy;
    s32 capacity;
    s32 primaryCount;
};

/* The seven pools (func_800B06E0 initialises them; func_800AF818 owns D_80153AE4). */
extern ItemPool D_80143094, D_801430A4, D_801430B4, D_801430C4, D_801430D4, D_801430E4, D_801430F4;
extern ItemPool *const D_80153AE4[7];
/* Pool-exhaustion sentinel record (func_800AFA7C defines it as D_801C5670) and the record
 * that func_800B0164 publishes while restoring a pool (consumed by func_800AFA7C/AF950). */
extern Item *D_80143090;
extern Item *D_80143104;

/* Item-pool allocator. */
ItemPool *func_800AF890(ItemPool *pool, Item *storage, unsigned char *occupancy, s32 capacity);
void func_800AF8D0(ItemPool *pool);
void func_800AF910(ItemPool *pool, s32 used);
s32 func_800AF920(ItemPool *pool, unsigned char index);
s32 func_800AF950(ItemPool *pool);
s32 func_800AFA18(ItemPool *pool);
Item *func_800AFA7C(ItemPool *pool);
Item *func_800AFAFC(ItemPool *pool);
ItemPool *func_800AFB80(Item *item);
s32 func_800AFBFC(Item *item);
void func_800AFC6C(ItemPool *pool, Item *item);
void func_800AFCA8(ItemPool *pool, unsigned char index);
void func_800AFCD8(ItemPool *pool, unsigned char index);
s32 func_800AFD08(ItemPool *pool, Item *item);
Item *func_800AFD78(ItemPool *pool, unsigned char index);
s32 func_800AFDBC(ItemPool *pool);
Item *func_800AFE30(ItemPool *pool, Item *source);
void func_800AFEB4(ItemPool *owner, Item *source, ItemPool *pool, unsigned char index);
unsigned char func_800AFFD0(ItemPool *source, Item *item, ItemPool *destination);
/* Index of a pool in D_80153AE4, or -1. */
s32 func_800AF7E0(ItemPool *pool);
/* One of the seven pools listed in D_80153AE4, or 0. */
ItemPool *func_800AF818(u32 index);
/* Destroys every live id-0xF4 item of the pool. */
void func_800B0348(ItemPool *pool);
/* The container object D_80147FA0 (pool D_801430B4) or D_80147FAC (pool D_801430C4). */
char *func_800D4EC4(ItemPool *pool);
/* Item containers (vtable D_80154668, constructor func_800D0650) hold their pool at +0;
 * their methods pass the address of that field. Slot of the `index`-th live record, then of
 * the vacant ones after all live records; the capacity when there is none. */
u32 func_800D07D8(ItemPool **pool, s32 index);
/* Removes entry `index` from a kind-3/4 item's seal list; 1 on success. */
s32 func_8010BE60(Item *item, unsigned char index);

/* Pool handle (8 bytes): the item pool that the base constructor func_8013687C clears, then
 * the method table. Base vtable D_80149DB8 (destructor func_8013684C); derived pool handles
 * use D_80149DA8 (destructor func_8013680C clears the pool; func_800D5160/52D0/5374/5484/
 * 5514, func_8012286C), the floor grid D_80154828 (func_800D4C44/4C8C; func_800D4CE4 binds
 * pool D_801430A4) and the storage object D_80154848 (func_800D4F14/4F5C). The base
 * methods below read the pool at +0 (capacity +8 in func_800D4A10, func_800AF920/AFD78 in
 * func_800D4A60/4AE8, func_800AFEB4 in func_800D4A28). Vtable contents are not modelled.
 * D_80147FD0 lists the three pools D_801430D4/E4/F4 (ROM 0x11B010). */
typedef struct HandleVTable HandleVTable;
typedef struct Handle {
    ItemPool *pool;
    const HandleVTable *vtable;
} Handle;
extern const HandleVTable D_80149DA8, D_80149DB8, D_80154828, D_80154848;
extern ItemPool *D_80147FD0[3];
void func_8013687C(ItemPool **pool);
/* Item `key` of the handle's pool moved into pool D_80143094, or 0 (func_800D4A60);
 * store/release record `index` through func_800AFEB4 (func_800D4A28); destroy item
 * `key` (func_800D4AE8); func_800D4A60 for index < 3, else 0 (func_800D55F0). */
Item *func_800D4A60(Handle *handle, s32 key);
void func_800D4A28(Handle *handle, s32 index, Item *item);
void func_800D4AE8(Handle *handle, s32 key);
Item *func_800D55F0(Handle *handle, u32 index);
/* Item `index` of the handle's pool, or 0 when absent or unusable. */
Item *func_800D561C(Handle *handle, s32 index);
/* Whether item 2 of pool D_80147FD0[(u8)index] is present and usable. */
s32 func_800D5484(s32 index);
/* Stores `item` as record `index` (0..3) of pool D_80147FD0[0] (func_800D4A28). */
void func_800D52D0(Item *item, s32 index);

/* Selected-item cursor (12 bytes): owning collection, selected item, and a selector word
 * whose low byte (+0xB) is the sub-index (func_800A0DA8 stores the collection +0, clears
 * +4 and stores -1 at +8). func_800A1218 reads +4 and +0xB; the interpreter's static
 * cursor D_801F6F50 is this object. The operand union records the word/half/byte widths
 * the interpreter reads from operand storage. */
typedef struct ItemCollection ItemCollection;
#include "script_operand.h"
typedef struct ItemCursor {
    ItemCollection *collection;
    Item *item;
    Operand index;
} ItemCursor;
/* Removes the selected seal (func_8010BE60) and returns the item's kind byte (lbu +0). */
s32 func_800A1218(ItemCursor *cursor);

/* Stores its halfword argument in D_801480CC (sh at 0x800D74A0); the global is read back
 * with lhu at 0x800D7698, and the interpreter passes an lhu operand half. */
void func_800D7444(unsigned short id);

#ifdef __cplusplus
}
#endif
#endif
