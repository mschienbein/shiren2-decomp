#include "common.h"
typedef struct { s32 field_00, field_04; void *field_08; unsigned short field_0C, field_0E; void *field_10; u32 field_14; void *field_18; void *field_1C; void *field_20; s32 field_24; unsigned char field_28[0x18]; void *field_40; } Config;
typedef struct { unsigned char field_00[0xC9]; unsigned char field_C9; unsigned char field_CA[0x72]; } Entry;
typedef struct { unsigned char field_00[0x1C]; } Record;
typedef struct { unsigned short field_00, field_02; unsigned char field_04; } Params;
/* Player client shared with the handler dispatch in func_80130614. */
typedef struct Callback {
    struct Callback *next;
    void *clientData;
    s32 (*update)(struct Callback *);
    s32 field_0C;
    s32 end;
} Callback;
extern void *D_801D2C28;
extern s32 D_801DE978, D_80000300, D_801CA6D4, D_801CA6E4, D_801CA6E8;
extern s32 D_801CA704, D_801CA6F0, D_801CA6F4;
extern struct Record_8012A7C4 *D_801CA6F8;
extern void *D_801CA6FC, *D_801CA70C, *D_801CA708;
extern void (*D_801CA710)(s32, s32);
extern Entry *D_801CA6DC, *D_801CA6E0;
extern Record *D_801CA6D8;
extern Callback D_801CA6C0;
extern void func_8012D800(void *, u32), func_8012D3A0(void *);
extern void *func_8012D84C(s32);
extern void func_8012AAC0(s32), func_8012A75C(void *, void *), func_8012A910(void *), func_8012D550(Config *, s32, s32), func_80129EB4(s32, s32), func_8012FDE0(Callback *), func_8012C004(Entry *);
extern s32 func_8012ACC0(Callback *);
extern s32 func_8012FE30(Record *, Params *);
extern s32 func_8012D87C(void);
void func_80129C2C(Config *config) {
    s32 i;
    D_801D2C28 = config->field_40; D_801DE978 = config->field_00; D_801CA6D4 = config->field_04 + 4;
    if (D_80000300) D_801CA6E4 = 60; else D_801CA6E4 = 50;
    D_801CA6E8 = 1000000 / D_801CA6E4;
    func_8012D800(config->field_10, config->field_14); func_8012D3A0(config->field_08);
    D_801CA6D8 = func_8012D84C((D_801CA6D4 - 4) * sizeof(Record));
    D_801CA6DC = func_8012D84C(D_801CA6D4 * sizeof(Entry)); D_801CA6E0 = D_801CA6DC + 4;
    func_8012AAC0(config->field_24);
    D_801CA6F8 = 0; D_801CA6FC = 0;
    if (config->field_18 && config->field_1C) func_8012A75C(config->field_18, config->field_1C);
    D_801CA70C = 0; D_801CA708 = 0;
    if (config->field_20) func_8012A910(config->field_20);
    D_801CA710 = 0; D_801CA704 = 2;
    func_8012D550(config, D_801CA6E4, 2); func_80129EB4(3, 0x7FFF);
    D_801CA6F0 = 1; D_801CA6F4 = 0x12345678;
    D_801CA6C0.next = 0; D_801CA6C0.update = func_8012ACC0; D_801CA6C0.clientData = &D_801CA6C0;
    func_8012FDE0(&D_801CA6C0);
    for (i = 0; i < D_801CA6D4; i++) { Params params; D_801CA6DC[i].field_C9 = 0; func_8012C004(&D_801CA6DC[i]); params.field_04 = 0; params.field_00 = config->field_0E; params.field_02 = 0; if (i >= 4) func_8012FE30(&D_801CA6D8[i - 4], &params); }
    func_8012D87C();
}
