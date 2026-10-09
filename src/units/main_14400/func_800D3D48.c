#include "common.h"
typedef struct { unsigned char field_0[0x80]; void *field_80; } Object;
typedef struct { unsigned char field_0[5]; signed char field_5; } Item;
/* Two 0x18-byte records (splat labels D_80143330 and D_80143348): owner byte +0,
 * area pointer +4 (func_800D2A64), remaining bytes not interpreted here. */
typedef struct { signed char owner; unsigned char pad1[3]; void *area; unsigned char pad8[0x10]; } Record;
/* Entry iterator built by func_800B07F0: collection pointer +0, halfword cursor +4. */
typedef struct { void *collection; unsigned short cursor; } Iterator;
extern Record D_80143330[2];
extern unsigned char D_80143448;
extern s32 func_800A9070(s32 *, s32);
extern Object *func_800A910C(s32 *);
extern void func_800F61FC(Object *);
extern void func_800D37E8(s32);
extern Iterator *func_800B07F0(Iterator *);
extern s32 func_800B0808(Iterator *);
extern Item *func_800B0864(Iterator *);
extern void func_800AE974(Item *, signed char);
extern s32 func_800D2FB0(void *);
extern void func_800D3698(s32, s32);
static inline s32 *init_cursor(s32 *cursor) { *cursor = 0; return cursor; }
void func_800D3D48(void) {
    Iterator other;
    s32 iterator;
    s32 *cursor = init_cursor(&iterator);
    for (;;) {
        Object *object;
        if (!func_800A9070(cursor, 0x57)) break;
        object = func_800A910C(cursor);
        object->field_80 = &D_80143330[0];
        func_800F61FC(object);
    }
    func_800D37E8(1);
    if (D_80143448 == 2) {
        func_800B07F0(&other);
        for (;;) {
            Item *item;
            if (!func_800B0808(&other)) break;
            item = func_800B0864(&other);
            if (item->field_5 == 1) func_800AE974(item, 0);
        }
        func_800D3698(0, func_800D2FB0(&D_80143330[1]));
    }
}
