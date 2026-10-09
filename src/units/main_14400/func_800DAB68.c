#include "common.h"

typedef struct Obj Obj;
typedef struct { unsigned char pad00[0x98]; short adjust98; short pad9A; Obj *(*get9C)(void *); } VTable;
typedef struct { unsigned char pad00[0x1E]; unsigned char flags1E; unsigned char pad1F[5]; VTable *vtable24; } Actor;
extern Actor *D_801476B8;
extern void *func_800AFB80(void *obj);
extern s32 func_800AF7E0(void *value);
extern char *func_800D4EC4(char *p);
extern Obj *func_800DAC60(Obj *a, Obj *b);
extern Obj *func_800EBA54(Actor *obj);
extern void *func_800A6D40(Actor *p);
extern void *func_800A6DE4(Actor *obj);
extern void *func_800EFB38(Actor *obj);

void *func_800DAB68(void *item)
{
    void *container = func_800AFB80(item);
    Obj *found;
    Actor *actor;
    if (func_800AF7E0(container)) return func_800D4EC4(container);
    found = func_800DAC60(D_801476B8->vtable24->get9C(
        (char *)D_801476B8 + D_801476B8->vtable24->adjust98), item);
    if (found) return found;
    found = func_800DAC60(func_800EBA54(D_801476B8), item);
    if (found) return found;
    actor = func_800A6D40(D_801476B8);
    if (actor != 0) {
        found = func_800DAC60(func_800A6DE4(actor), item);
        if (found) return found;
        if ((actor->flags1E >> 6) & 1) return func_800EFB38(actor);
    }
    return 0;
}
