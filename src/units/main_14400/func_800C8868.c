#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef struct { unsigned short high:9; unsigned short blocked:1; unsigned short low:6; } Flags;
typedef struct { unsigned char high:5; unsigned char blocked:1; unsigned char low:2; } ItemFlags;
typedef struct Item { unsigned char field_0[0x1E]; ItemFlags field_1E; unsigned char field_1F[0x33]; unsigned char field_52, field_53, field_54, field_55; } Item;
typedef struct { unsigned char field_0[0x104]; Item *field_104; } Object;
extern Flags D_8014767C;
extern s32 D_80147678;
extern Object *D_801476B8;
extern s32 func_80046240(void);
extern s32 func_800A8FC8(s32 *, s32);
extern Item *func_800A910C(s32 *);
extern unsigned short func_800E08B0(Item *);
extern s32 func_800A08D8(s32, s32, s32);
extern s32 func_80049CB4(s32, ...);
extern void func_800C8DE8(Item *);
static inline s32 is_current(Item *item) { s32 result = 0; Item *current = D_801476B8->field_104; if (current) result = item == current; return result; }
static inline s32 state_differs(void) { return D_80142F18.mode ^ 0x4F; }
static inline s32 globally_blocked(void) { return D_8014767C.blocked; }
static inline s32 item_blocked(Item *item) { return item->field_1E.blocked; }
static inline s32 skip_item(s32 priority, Item *item) {
    s32 skip = 0;
    if ((item->field_55 >> 4) != priority || !func_800E08B0(item) || !(item->field_54 & 4) || item->field_52 || item_blocked(item) || is_current(item)) skip = 1;
    return skip;
}
/* ODD_C: Keep the priority test at the outer traversal boundary. */
static inline s32 valid_priority(s32 priority) { return priority < 4; }
void func_800C8868(s32 notify) {
    s32 blocked = 0;
    if ((func_80046240() && state_differs()) || globally_blocked()) blocked = 1;
    if (!blocked) {
        s32 priority;
        s32 active = 0;
        s32 iterator = 0;
        s32 *cursor;
        priority = 0;
        cursor = &iterator;
        for (; ; ++priority) {
            s32 keep_going = valid_priority(priority);
            if (!keep_going) break;
            iterator = 0;
            for (;;) {
                Item *item;
                s32 skip;
                if (!func_800A8FC8(cursor, 0x7C)) break;
                item = func_800A910C(cursor);
                skip = skip_item(priority, item);
                if (skip) continue;
                if (notify) { notify = 0; func_800A08D8(1, -1, 2); func_80049CB4(2); }
                if (!active) { func_80049CB4(0x10, 1); active++; }
                func_800C8DE8(item);
                func_80049CB4(4);
                func_80049CB4(3);
                if (D_80147678 || globally_blocked()) {
                    if (active) func_80049CB4(0x10, 0);
                    return;
                }
            }
        }
        if (active) func_80049CB4(0x10, 0);
    }
}
