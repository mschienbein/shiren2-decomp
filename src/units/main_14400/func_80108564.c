#include "common.h"

typedef unsigned char u8;
typedef struct {
    void *pool;
    void *vtable;
    u8 *items;
    u8 capacity;
    u8 limit;
    u8 count;
} CollectionBase;
typedef struct {
    CollectionBase base;
    void *owner;
    unsigned short kind;
} Collection;
typedef struct {
    char pad[0x24];
    void *unk24;
    char pad28[0x78];
    u8 itemsA0[3];
    Collection unkA4;
    s32 unkBC;
    s32 unkC0;
    s32 unkC4;
} Obj;
extern s32 D_8015C450;
void func_800CE6A0(CollectionBase *, s32);
void func_800EFD28(Obj *, s32);
void func_800A3918(Obj *);
void func_80108564(Obj *obj, s32 flags) {
    obj->unk24 = &D_8015C450;
    func_800CE6A0(&obj->unkA4.base, 2);
    func_800EFD28(obj, 0);
    if (flags & 1) {
        func_800A3918(obj);
    }
}
