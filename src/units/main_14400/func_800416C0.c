#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    s32 key;
    s32 owner;
} Query_800416C0;

extern u8 *func_800B4D80(Query_800416C0 *query);

u8 func_800416C0(s32 owner, s32 key) {
    Query_800416C0 query;
    u8 *entry;

    query.owner = owner;
    query.key = key;
    entry = func_800B4D80(&query);
    if (entry != 0) {
        return *entry;
    }
    return 0;
}
