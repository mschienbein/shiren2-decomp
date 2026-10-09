#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 words[12];
} Record800AFE30;

typedef struct Pool800AFE30 Pool800AFE30;

/* Pool exhaustion sentinel returned by func_800AFA7C. */
extern Record800AFE30 *D_80143090;

extern Record800AFE30 *func_800AFA7C(Pool800AFE30 *pool);
extern void func_800AE648(Record800AFE30 *obj);

/* Take a record from the pool, copy src into it and register it; 0 when the pool is full. */
Record800AFE30 *func_800AFE30(Pool800AFE30 *pool, Record800AFE30 *src) {
    Record800AFE30 *record = func_800AFA7C(pool);

    if (record == D_80143090) {
        return 0;
    }
    *record = *src;
    func_800AE648(record);
    return record;
}
