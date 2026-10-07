#include "common.h"

typedef struct { short delta; short index; void (*fn)(void *, s32, void *); } VEntry;
typedef struct { char pad[0x28]; VEntry e; } VTable;
typedef struct { char pad[0x18]; VTable *vtbl; } Obj;
extern char D_8014A958[];
void func_800CA4E8(Obj *, void *);
void func_80045984(s32, unsigned char);
void func_80046098(Obj *obj) {
    func_800CA4E8(obj, D_8014A958);
    for (;;) {
        s32 id;
        unsigned char val;
        obj->vtbl->e.fn((char *)obj + obj->vtbl->e.delta, 4, &id);
        if (id == -1) break;
        obj->vtbl->e.fn((char *)obj + obj->vtbl->e.delta, 1, &val);
        func_80045984(id, val);
    }
}
