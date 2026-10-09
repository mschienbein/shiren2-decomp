#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct { u8 a; u8 b; } Pair8;
typedef struct { s32 a, b; } Key;
/* Query result record (func_80044768); D_80138B40 is the null record. */
typedef struct { s32 id; Key key; } End;
typedef struct { End end; s32 foundC; } Query;
/* Sound request built by func_80045370 (0x18 bytes, kind byte at +4). */
typedef struct { Pair8 *pos; u8 kind, padding[3]; Key field08, field10; } Request;
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;

extern s32 D_80138BD0;
extern End D_80138B40;
extern SelectionRecord D_80142F18;
extern Query *func_80044730(Query *query);
End *func_80044768(Query *query);
u8 func_80052694(s16 id);
void func_80045370(Request *request, Key *key, Pair8 mode);
Pair8 func_800453B8(Request *request);
Pair8 func_80045468(Request *request);
s32 func_800524F4(void);
void func_80045AAC(void);
s32 func_8005276C(void);
s32 func_8005278C(void);
void func_80052518(s32 handle, Pair8 res);
void func_80051D94(s16 id, Pair8 res);

/* Kind 4 marks a request played immediately rather than queued. */
static inline Pair8 play_now(Request *request)
{
    request->kind = 4;
    return func_800453B8(request);
}

void func_80045E50(void)
{
    Pair8 res;
    Pair8 mode;
    Query query;
    Request request;
    Pair8 tmp;
    End *entry;
    s16 id;
    s32 busy;

    if (D_80138BD0 == 0) {
        return;
    }
    func_80044730(&query);
    entry = func_80044768(&query);
    if (entry->id == D_80138B40.id) {
        if (func_800524F4() != 0) {
            func_80045AAC();
        }
        return;
    }
    mode.a = func_80052694(entry->id);
    mode.b = 0x80;
    tmp = mode;
    func_80045370(&request, &entry->key, tmp);
    busy = ((D_80142F18.flags >> 2) & 1) ^ 1;
    /* ODD_C: each arm copies its own call result into res; GCC cross-jumps the
       two copies into one tail, and keeping the copy in the call's block lets
       sched1 order the kind store after the argument moves (stored through a1). */
    if (busy) {
        tmp = play_now(&request);
        res = tmp;
    } else {
        tmp = func_80045468(&request);
        res = tmp;
    }
    if (res.a < 0x1E) {
        if (func_800524F4() != 0) {
            func_80045AAC();
        }
        return;
    }
    id = entry->id;
    if (id == (s16)func_8005276C()) {
        if (func_800524F4() != 0) {
            func_80052518(func_8005278C(), res);
        } else {
            func_80051D94(id, res);
        }
    } else {
        func_80051D94(id, res);
    }
}
