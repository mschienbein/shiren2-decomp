/* func_801EA990_13C8B0 (overlay_136dc0, ROM 0x13C8B0, 280 bytes).
 * Author: opus-09 (Anthropic, claude-opus-5-5, xhigh), wave39 private candidate.
 * Object views copied from canonical src/units/overlay_136dc0/func_801EA810_13C730/types.h
 * (OvlCollectionTable/OvlCollection/OvlItemTable/OvlItem, unchanged field offsets). */
typedef int s32;
typedef unsigned int u32;
typedef short s16;
typedef unsigned char u8;

typedef struct OvlItem OvlItem;
typedef struct OvlCollectionTable {
    u8 pad00[0x18];
    s16 delta18;
    u8 pad1A[2];
    void (*method1C)(void *, s32);
    s16 delta20;                     /* lh 0x20 at ROM 0x13C928 */
    u8 pad22[2];
    s32 (*method24)(void *);         /* lw 0x24 / jalr at ROM 0x13C92C..0x13C930 */
    u8 pad28[0x10];
    s16 delta38;                     /* lh 0x38 at ROM 0x13C954 */
    u8 pad3A[2];
    OvlItem *(*method3C)(void *, u32); /* lw 0x3C / jalr at ROM 0x13C958..0x13C95C */
} OvlCollectionTable;
typedef struct OvlCollection {
    void *field00;
    OvlCollectionTable *field04;     /* lw 4 at ROM 0x13C924/0x13C94C */
    u8 pad08[0x10];
    void *field18;
} OvlCollection;
typedef struct OvlItemTable {
    u8 pad00[8];
    s16 delta08;
    u8 pad0A[2];
    void (*method0C)(void *, s32);
    u8 pad10[0x28];
    s16 delta38;
    u8 pad3A[2];
    s32 (*method3C)(void *, void *);
} OvlItemTable;
struct OvlItem {
    u8 field00;                      /* lbu 0 kind at ROM 0x13C904 */
    u8 field01;                      /* lbu 1 at ROM 0x13C968 (container id 0xA6) */
    u8 pad02[2];
    u8 field04;
    u8 pad05[3];
    OvlItemTable *field08;
    union {
        u8 byte;
        s32 word;
        OvlCollection collection;    /* &container->field0C at ROM 0x13C97C */
    } field0C;
};

extern OvlCollection *func_801EA2D8_13C1F8(s32 owner);
extern void func_801EA810_13C730(OvlItem *item, s32 value);
extern s32 func_800CD538(void *collection, OvlItem *item); /* canonical: (void *self, Obj *obj) */
extern s32 func_800CD278(void *object);                    /* canonical: (Object *) */
extern void *func_8011422C(OvlItem *item);                 /* canonical: void *(Obj8011422C *) */
extern s32 func_800CD5C0(void *collection, OvlItem *item); /* canonical: (void *self, Obj *obj) */

/* Returns 1 when func_800CD538 accepts `item` for `owner`'s collection, or when a
 * 0xA6 container entry (scanned from the last index down) has a nonzero
 * func_800CD278 count and func_800CD5C0 returns 1 for its func_8011422C target.
 * `value` is applied first through func_801EA810_13C730; kinds 9 and 17 skip the
 * container scan. Same while(1)/break loop shape as func_801EA728_13C648: the exit
 * test breaks to the trailing `return 0`, so the success `return 1` falls into the
 * epilogue. */
s32 func_801EA990_13C8B0(s32 owner, OvlItem *item, s32 value)
{
    OvlCollection *collection = func_801EA2D8_13C1F8(owner);
    s32 i;
    OvlItem *container;

    if (!collection) {
        return 0;
    }
    if (value) {
        func_801EA810_13C730(item, value);
    }
    if (func_800CD538(collection, item)) {
        return 1;
    }
    if (item->field00 == 9) {
        return 0;
    }
    if (item->field00 == 17) {
        return 0;
    }
    i = collection->field04->method24((u8 *)collection + collection->field04->delta20);
    while (1) {
        if (--i == -1) {
            break;
        }
        container = collection->field04->method3C((u8 *)collection + collection->field04->delta38, i);
        if (container->field01 == 0xA6 && func_800CD278(&container->field0C.collection)
            && func_800CD5C0(func_8011422C(container), item) == 1) {
            return 1;
        }
    }
    return 0;
}
