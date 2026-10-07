#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    void (*handler_0)(void *);
    s16 mode_4;
    u16 id_6;
    s16 field_8;
    s16 field_A;
    u8 padC[0x4];
    u16 field_10;
    u8 pad12[0x74 - 0x12];
} Entry80084CD4;

extern Entry80084CD4 D_801BA380[];
extern s32 D_801BF2C0;
extern u16 D_801BF2C2;
extern s32 D_801BF2C8;
extern s32 D_8013E914;

u16 func_80084BCC(void);
s32 func_80084A84(void);
void func_80084FD0(s32 a0);
void func_80084904(void);
void func_80084A90(s32 a0);

s32 D_8013E900 = 0;

s32 func_80084CD4(void (*handler)(void *)) {
    s32 id = func_80084BCC();
    Entry80084CD4 *entry;
    s32 savedCount;
    s32 savedMode;

    if (id == 0xAE) {
        func_80084FD0(func_80084A84());
        savedCount = D_801BF2C8;
        savedMode = D_8013E900;
        func_80084904();
        D_8013E900 = savedMode;
        func_80084A90(0);
        D_801BF2C8 = savedCount;
        id = func_80084BCC();
    }
    entry = &D_801BA380[id];
    entry->handler_0 = handler;
    entry->id_6 = id;
    if (D_8013E900 != 1) {
        entry->mode_4 = 0;
    } else {
        entry->mode_4 = 2;
        if (D_8013E914 != 0) {
            entry->field_10 = D_801BA380[D_801BF2C0].field_10;
            D_8013E914 = 0;
        } else {
            entry->field_10 = D_801BF2C2;
        }
    }
    entry->field_A = 0;
    entry->field_8 = 0;
    D_801BF2C0 = id;
    D_801BF2C8++;
    return id;
}
