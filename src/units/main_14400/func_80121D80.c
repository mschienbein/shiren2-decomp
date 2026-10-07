#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { short delta; short pad; s32 (*fn)(void *); } CountEntry;
typedef struct { short delta; short pad; void *(*fn)(void *, u32); } GetEntry;
typedef struct { short delta; short pad; void *(*fn)(void *, void *); } NameEntry;
typedef struct {
    char pad0[0x20];
    CountEntry count;
    char pad28[0x10];
    GetEntry get;
} SubVTable;
typedef struct { char pad0[0x30]; NameEntry name; } ItemVTable;
typedef struct { s32 unk0; SubVTable *vtable; } Sub;
typedef struct { char pad0[0xC]; Sub sub; char pad14[0x18]; u8 unk2C; } Obj;
typedef struct { char pad0[8]; ItemVTable *vtable; char padC[3]; u8 unkF; } Item;
typedef struct { char pad0[0x9D]; u8 unk9D; } Ent;
extern char D_801CA640[];
Ent *func_801217DC(Obj *);
char *func_800A3CD0(void *obj);
char *func_80083C90(char *dst, char *src);
char *func_80048480(u16 id);
s32 func_800327C0(char *dst, const char *fmt, ...);
char *func_80083D04(char *dst, char *src);
char *func_80121D80(Obj *obj, char *buf) {
    s32 empty = 0;
    u8 count;
    if (obj->unk2C == 0xFF) {
        Sub *sub = &obj->sub;
        empty = sub->vtable->count.fn((char *)sub + sub->vtable->count.delta) == 0;
    }
    if (empty) {
        return 0;
    }
    {
        Sub *sub = &obj->sub;
        if (sub->vtable->count.fn((char *)sub + sub->vtable->count.delta) != 0) {
            Item *item = sub->vtable->get.fn((char *)sub + sub->vtable->get.delta, 0);
            item->vtable->name.fn((char *)item + item->vtable->name.delta, buf);
            count = item->unkF;
        } else {
            Ent *ent = func_801217DC(obj);
            func_80083C90(buf, func_800A3CD0(ent));
            count = ent->unk9D;
        }
    }
    if (count != 0) {
        func_800327C0(D_801CA640, func_80048480(0x249), count);
        func_80083D04(buf, D_801CA640);
    }
    return buf;
}
