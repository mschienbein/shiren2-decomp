#include "common.h"

typedef struct ObjA ObjA;
typedef struct ObjB ObjB;

typedef struct {
    ObjA *owner;
    ObjB *attached;
} Ref;

extern void func_800CD364(ObjA *a, ObjB *b);

void func_800D0348(Ref *ref)
{
    func_800CD364(ref->owner, ref->attached);
    ref->attached = 0;
}
