#include "common.h"
typedef short s16;
typedef struct { s16 delta; s16 index; void *fn; } VtblEntry;
typedef unsigned char u8;
typedef signed char s8;
typedef struct {
    u8 unk0;
    u8 group;
    char pad2[3];
    s8 slot;
    char pad6[2];
    VtblEntry *vtbl;
} Item800CDE78;
typedef struct { s32 unk0; VtblEntry *vtbl; } List800CDE78;
/* List slots: +0x24 s32 count(self), +0x3C void *get(self, u32 index), +0x54 void move(self, s32 from, s32 to)
 * (as in func_800CD304/func_800CEBA0/func_800CD764); item slot +0x1C s32 test(self, s32 kind). */
s32 func_800CDD68(Item800CDE78 *);
s32 func_800CD2BC(List800CDE78 *, Item800CDE78 *);
s32 func_800CD538(List800CDE78 *, Item800CDE78 *);
void func_800CE0F4(List800CDE78 *);

static inline s32 listCount(List800CDE78 *list) {
    VtblEntry *e = &list->vtbl[4];
    return ((s32 (*)(void *))e->fn)((char *)list + e->delta);
}

static inline Item800CDE78 *listGet(List800CDE78 *list, u32 index) {
    VtblEntry *e = &list->vtbl[7];
    return ((void *(*)(void *, u32))e->fn)((char *)list + e->delta, index);
}

static inline void listMove(List800CDE78 *list, s32 from, s32 to) {
    VtblEntry *e = &list->vtbl[10];
    ((void (*)(void *, s32, s32))e->fn)((char *)list + e->delta, from, to);
}

static inline s32 itemCheck(Item800CDE78 *item, s32 arg) {
    VtblEntry *e = &item->vtbl[3];
    return ((s32 (*)(void *, s32))e->fn)((char *)item + e->delta, arg);
}

void func_800CDE78(List800CDE78 *list) {
    s32 count = listCount(list);
    s32 i;
    s32 j;

    if (count == 0) {
        return;
    }
    i = 1;
    while (1) {
        Item800CDE78 *item;
        u32 key;
        u8 group;

        if (!(i < count)) {
            break;
        }
        item = listGet(list, i);
        key = func_800CDD68(item);
        group = item->group;
        j = 0;
        while (1) {
            Item800CDE78 *other;
            u32 otherKey;

            if (!(j < i)) {
                break;
            }
            other = listGet(list, j);
            otherKey = func_800CDD68(other);
            if (key == otherKey && other->group == group) {
                j++;
                while (1) {
                    s32 differs;

                    if (!(j < i)) {
                        break;
                    }
                    other = listGet(list, j);
                    differs = 0;
                    if (key != func_800CDD68(other) || other->group != group) {
                        differs = 1;
                    }
                    if (differs) {
                        break;
                    }
                    j++;
                }
                if (j != i) {
                    listMove(list, i, j);
                }
                break;
            }
            if (key < otherKey) {
                listMove(list, i, j);
                break;
            }
            j++;
        }
        i++;
    }

    count = listCount(list);
    while (1) {
        Item800CDE78 *item;
        s32 skip;
        u8 group;

        count--;
        if (count == -1) {
            break;
        }
        item = listGet(list, count);
        skip = 0;
        if (!itemCheck(item, 0x1E)) {
            skip = 1;
        } else if (~item->slot != 0) {
            skip = 1;
        }
        if (!skip) {
            s32 k;

            group = item->group;
            k = count;
            while (1) {
                Item800CDE78 *other;
                s32 differs;

                k--;
                if (k == -1) {
                    break;
                }
                other = listGet(list, k);
                differs = 0;
                if (other->group != group || ~other->slot != 0) {
                    differs = 1;
                }
                if (!differs) {
                    func_800CD2BC(list, item);
                    func_800CD538(list, item);
                    break;
                }
            }
        }
    }
    func_800CE0F4(list);
}
