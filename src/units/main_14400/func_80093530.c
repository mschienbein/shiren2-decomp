#include "common.h"

typedef unsigned char u8;

/* Container/item link produced by the struct-returning queries. */
typedef struct { void *container; void *item; } Pair;
typedef struct Context Context;
typedef struct Command Command;
typedef struct Menu Menu;
typedef struct Picker Picker;
typedef struct Record Record;

extern Menu D_80141A28, D_80140CF8, D_801408EC;
extern Record D_80141AD0;
extern Picker D_80141994;

extern void *func_800D8FB0(u32 size);
extern Pair func_8009AB34(Menu *request);
extern Pair func_80097F20(Menu *menu);
extern u8 *func_8009C17C(Record *record);
extern unsigned char func_8009E088(Picker *picker);
extern Command *func_800DAE40(void *obj, Context *ctx);
extern Command *func_800DAF10(void *obj, Context *ctx);
extern Command *func_800DAFE0(void *obj, Context *ctx);
extern Command *func_800DB1F8(void *obj, Context *ctx);
extern Command *func_800DB870(void *obj, Context *ctx);
extern Command *func_800DB9A0(void *obj, Context *ctx);
extern Command *func_800DBB90(void *obj, Context *ctx);
extern Command *func_800DC2A0(void *obj, Context *ctx);
extern Command *func_800DC3F0(void *obj, Context *ctx);
extern Command *func_800DC730(void *obj, Context *ctx);
extern Command *func_800DC9E0(void *obj, Context *ctx);
extern Command *func_800DCB00(void *obj, Context *ctx);
extern Command *func_800DCCC0(void *obj, Context *ctx);
extern Command *func_800DD320(void *obj, Context *ctx);
extern Command *func_800DD3F0(void *obj, Context *ctx);
extern Command *func_800DD4C0(void *obj, Context *ctx);
extern Command *func_800DD590(void *obj, Context *ctx);
extern Command *func_800DD6E0(void *obj, Context *ctx);
extern Command *func_800DD880(void *obj, Context *ctx);
extern Command *func_800DD970(void *obj, Context *ctx);
extern Command *func_800DB3AC(void *obj, Context *ctx, Pair *link);
extern Command *func_800DB7DC(void *obj, Context *ctx, Pair *link);
extern Command *func_800DBD00(void *obj, Context *ctx, Pair *link);
extern Command *func_800DBFF0(void *obj, Context *ctx, Pair *link);
extern Command *func_800DC4C0(void *obj, Context *ctx, Pair *link);
extern Command *func_800DCE60(void *obj, Context *ctx, u8 *code);
extern Command *func_800DD180(void *obj, Context *ctx, unsigned char value);
extern Command *func_800DCFF0(void *obj, Context *ctx, u8 *code);

/* Command factory: allocates and constructs the command object for `type`.
 * The first parameter is supplied by callers but unused. */
Command *func_80093530(void *unused, s32 type, Context *ctx)
{
    switch (type) {
    case 12:
        return func_800DAE40(func_800D8FB0(0x10), ctx);
    case 13:
        return func_800DAF10(func_800D8FB0(0x10), ctx);
    case 14:
        return func_800DAFE0(func_800D8FB0(0x10), ctx);
    case 15:
        return func_800DB1F8(func_800D8FB0(0x10), ctx);
    case 16:
    {
        void *obj = func_800D8FB0(0x18);
        Pair link = func_8009AB34(&D_80141A28);
        return func_800DB3AC(obj, ctx, &link);
    }
    case 17:
    {
        void *obj = func_800D8FB0(0x18);
        Pair link = func_80097F20(&D_80140CF8);
        return func_800DB7DC(obj, ctx, &link);
    }
    case 18:
        return func_800DB870(func_800D8FB0(0x10), ctx);
    case 19:
        return func_800DB9A0(func_800D8FB0(0x10), ctx);
    case 20:
        return func_800DBB90(func_800D8FB0(0x10), ctx);
    case 21:
    {
        void *obj = func_800D8FB0(0x18);
        Pair link = func_80097F20(&D_801408EC);
        return func_800DBD00(obj, ctx, &link);
    }
    case 22:
    {
        void *obj = func_800D8FB0(0x18);
        Pair link = func_80097F20(&D_801408EC);
        return func_800DBFF0(obj, ctx, &link);
    }
    case 23:
        return func_800DC2A0(func_800D8FB0(0x10), ctx);
    case 24:
        return func_800DC3F0(func_800D8FB0(0x10), ctx);
    case 25:
    {
        void *obj = func_800D8FB0(0x18);
        Pair link = func_80097F20(&D_801408EC);
        return func_800DC4C0(obj, ctx, &link);
    }
    case 26:
        return func_800DC730(func_800D8FB0(0x10), ctx);
    case 27:
        return func_800DC9E0(func_800D8FB0(0x10), ctx);
    case 28:
        return func_800DCB00(func_800D8FB0(0x10), ctx);
    case 29:
        return func_800DCCC0(func_800D8FB0(0x10), ctx);
    case 30:
    {
        void *obj = func_800D8FB0(0x14);
        return func_800DCE60(obj, ctx, func_8009C17C(&D_80141AD0));
    }
    case 31:
    {
        void *obj = func_800D8FB0(0x14);
        return func_800DD180(obj, ctx, func_8009E088(&D_80141994));
    }
    case 32:
    {
        void *obj = func_800D8FB0(0x14);
        return func_800DCFF0(obj, ctx, func_8009C17C(&D_80141AD0));
    }
    case 33:
        return func_800DD320(func_800D8FB0(0x10), ctx);
    case 34:
        return func_800DD3F0(func_800D8FB0(0x10), ctx);
    case 35:
        return func_800DD4C0(func_800D8FB0(0x10), ctx);
    case 36:
        return func_800DD590(func_800D8FB0(0x10), ctx);
    case 37:
        return func_800DD6E0(func_800D8FB0(0x10), ctx);
    case 38:
        return func_800DD880(func_800D8FB0(0x10), ctx);
    case 39:
        return func_800DD970(func_800D8FB0(0x10), ctx);
    }
    return 0;
}
