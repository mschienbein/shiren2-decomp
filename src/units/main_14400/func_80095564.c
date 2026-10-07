#include "common.h"
typedef unsigned char u8;
typedef signed short s16;
typedef struct { s32 index; s32 offset; } Cursor;
typedef struct {
    u8 pad[0x58];
    s16 readDelta; s16 readIdx; void (*read)(void *, s32, u8 *);
    s16 getDelta; s16 getIdx; s32 (*get)(void *, s32);
} VTable;
typedef struct { u8 pad0[0x20]; s32 count; u8 pad24[0x28]; VTable *vt; } Obj;
void func_80048870(Obj *o, s32 value);
void func_800487EC(Obj *o, s32 index, s32 n, u8 *buf);
static inline s32 cursorPos(Obj *o, Cursor *c) { return c->index + o->count * c->offset; }
static inline s32 objGet(Obj *o, s32 pos) { return o->vt->get((u8 *)o + o->vt->getDelta, pos); }
static inline void objRead(Obj *o, s32 pos, u8 *buf) { o->vt->read((u8 *)o + o->vt->readDelta, pos, buf); }
void func_80095564(Obj *o) {
    Cursor cursor;
    cursor.offset = 0;
    cursor.index = 0;
    for (;;) {
        u8 buf[0x400];
        if (cursor.index >= o->count) break;
        func_80048870(o, objGet(o, cursorPos(o, &cursor)));
        buf[0] = 0;
        objRead(o, cursorPos(o, &cursor), buf);
        func_800487EC(o, cursor.index, 1, buf);
        cursor.index++;
    }
}