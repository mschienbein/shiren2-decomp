#include "common.h"
/* g++ 2.x member-function descriptor: virtual index or typed direct function. */
typedef struct {
    signed short delta, index;
    union { void (*pfn)(void *); signed short delta2; } u;
} MemberFn;
/* Display and callback are distinct 0x10-byte embedded objects. */
typedef struct { unsigned char pad00[0x54]; unsigned char display54[0x10]; unsigned char callback64[0x10]; } Object;
extern const MemberFn D_80152CAC;
extern unsigned char D_80139010[];
extern void func_80095E58(void *, void *, MemberFn);
extern void func_800486A4(void *, void *, void *);
void func_8009E190(Object *obj) {
    void *part = obj->callback64;
    func_80095E58(part, obj, D_80152CAC);
    func_800486A4(obj->display54, D_80139010, part);
}
