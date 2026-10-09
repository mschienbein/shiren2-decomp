#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 id, weight; } Pick;
typedef struct { u8 id, weight; u16 pad2; void *data; } S;
struct Rng;
extern struct Rng D_80147620;
u16 func_800C58DC(void *rng, u16 range);
char *func_800AC064(S *entry);
s32 func_800AB2B0(Pick *table, s32 mode);
u8 func_800AB1C8(S *entry, u8 id, s32 mode) {
    if (!id) {
        S *scan = entry;
        s32 total = 0;
        u16 roll;
        while (scan->id) { total += scan->weight; scan++; }
        if (!(u16)total) return 0;
        roll = func_800C58DC(&D_80147620, total - 1);
        while (entry->data) {
            s32 weight = entry->weight;
            if ((u32)roll < (u32)weight) return func_800AB2B0((Pick *)func_800AC064(entry), mode);
            roll -= weight;
            entry++;
        }
    } else {
        while (entry->id) {
            if (entry->id == id) return func_800AB2B0((Pick *)func_800AC064(entry), mode);
            entry++;
        }
    }
    return 0;
}
