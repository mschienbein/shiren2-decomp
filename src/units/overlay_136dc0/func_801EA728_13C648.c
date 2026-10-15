/* func_801EA728_13C648 (overlay_136dc0, ROM 0x13C648, 232 bytes).
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
    s16 delta20;                     /* lh 0x20 at ROM 0x13C6B0 */
    u8 pad22[2];
    s32 (*method24)(void *);         /* lw 0x24 / jalr at ROM 0x13C6B4..0x13C6B8 */
    u8 pad28[0x10];
    s16 delta38;                     /* lh 0x38 at ROM 0x13C6DC */
    u8 pad3A[2];
    OvlItem *(*method3C)(void *, u32); /* lw 0x3C / jalr at ROM 0x13C6E0..0x13C6E4 */
} OvlCollectionTable;
typedef struct OvlCollection {
    void *field00;
    OvlCollectionTable *field04;     /* lw 4 at ROM 0x13C6AC/0x13C6D4 */
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
    u8 field00;
    u8 field01;                      /* lbu 1 at ROM 0x13C6F0 (container id 0xA6) */
    u8 pad02[2];
    u8 field04;
    u8 pad05[3];
    OvlItemTable *field08;
    union {
        u8 byte;
        s32 word;
        OvlCollection collection;    /* &item->field0C at ROM 0x13C704 */
    } field0C;
};

extern OvlCollection *func_801EA2D8_13C1F8(s32 owner);
extern s32 func_800AFDBC(void *pool);    /* canonical: s32 func_800AFDBC(ItemPool *) */
extern s32 func_800CD278(void *object);  /* canonical: s32 func_800CD278(Object *) */
extern u8 D_80143094[];                  /* item pool header passed by address */

/* Returns 0 without a collection or when func_800AFDBC(D_80143094) < amount;
 * 1 when func_800CD278(collection) >= amount; otherwise, only with `nested`, 1 when
 * some 0xA6 entry (scanned from the last index down) has func_800CD278 of its
 * embedded collection >= amount. The exit test is a `break` to the trailing
 * `return 0`, so the success `return 1` falls into the epilogue (ROM 0x13C714). */
s32 func_801EA728_13C648(s32 owner, s32 amount, s32 nested)
{
    OvlCollection *collection = func_801EA2D8_13C1F8(owner);
    s32 i;
    OvlItem *item;

    if (!collection) {
        return 0;
    }
    if (func_800AFDBC(D_80143094) < amount) {
        return 0;
    }
    if (func_800CD278(collection) >= amount) {
        return 1;
    }
    if (!nested) {
        return 0;
    }
    i = collection->field04->method24((u8 *)collection + collection->field04->delta20);
    while (1) {
        if (--i == -1) {
            break;
        }
        item = collection->field04->method3C((u8 *)collection + collection->field04->delta38, i);
        if (item->field01 == 0xA6 && func_800CD278(&item->field0C.collection) >= amount) {
            return 1;
        }
    }
    return 0;
}
