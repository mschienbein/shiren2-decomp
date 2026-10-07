#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    u8 pad0[0x14];
    s32 done_14;
} Ctx800C9CC4;

typedef struct {
    u8 pad0[0x28];
    s16 delta_28;
    s16 pad2A;
    void (*func_2C)(void *self, Ctx800C9CC4 *ctx);
} VTable800C9CC4;

typedef struct {
    u8 pad0[0x24];
    VTable800C9CC4 *vtable_24;
} Obj800C9CC4;

extern u8 D_801541CC[];
extern u8 D_80143094[];
extern Obj800C9CC4 *D_801476B8;

void func_800CA4E8(Ctx800C9CC4 *ctx, void *table);
void func_800A9608(Ctx800C9CC4 *ctx);
void func_800AEE58(Ctx800C9CC4 *ctx);
void func_800AF030(Ctx800C9CC4 *ctx);
void func_800B0164(void *table, Ctx800C9CC4 *ctx);
void func_800A8E40(Ctx800C9CC4 *ctx);
void func_801F2A70(Ctx800C9CC4 *ctx);
void func_800B6434(Ctx800C9CC4 *ctx);
void func_800EF868(Ctx800C9CC4 *ctx);
void func_800B77B4(Ctx800C9CC4 *ctx);
s32 func_800C9810(void);
void func_800462E8(Ctx800C9CC4 *ctx);
void func_80046098(Ctx800C9CC4 *ctx);

void func_800C9CC4(Ctx800C9CC4 *ctx) {
    func_800CA4E8(ctx, D_801541CC);
    func_800A9608(ctx);
    if (ctx->done_14 != 0) {
        return;
    }
    func_800AEE58(ctx);
    if (ctx->done_14 != 0) {
        return;
    }
    func_800AF030(ctx);
    if (ctx->done_14 != 0) {
        return;
    }
    func_800B0164(D_80143094, ctx);
    if (ctx->done_14 != 0) {
        return;
    }
    D_801476B8->vtable_24->func_2C((u8 *)D_801476B8 + D_801476B8->vtable_24->delta_28, ctx);
    if (ctx->done_14 != 0) {
        return;
    }
    func_800A8E40(ctx);
    if (ctx->done_14 != 0) {
        return;
    }
    func_801F2A70(ctx);
    if (ctx->done_14 != 0) {
        return;
    }
    func_800B6434(ctx);
    if (ctx->done_14 != 0) {
        return;
    }
    func_800EF868(ctx);
    if (ctx->done_14 != 0) {
        return;
    }
    func_800B77B4(ctx);
    if (ctx->done_14 != 0) {
        return;
    }
    if (func_800C9810() == 0) {
        return;
    }
    func_800462E8(ctx);
    if (ctx->done_14 != 0) {
        return;
    }
    func_80046098(ctx);
}
