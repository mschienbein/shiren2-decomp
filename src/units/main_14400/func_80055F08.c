#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct {
    u8 pad0[0x44];
    s32 id;
    u8 pad48[8];
    s16 handler_index;
    u8 pad52[0xA];
    s16 state;
    u8 pad5E[2];
} Ent80055F08;
extern s32 D_80139B30;
extern Ent80055F08 D_801D40DC[32];
extern void (*D_8013A24C[])(Ent80055F08 *ent);
void func_80056044(void);
void func_80055FD4(void);
void func_80055F08(void) {
    Ent80055F08 *ent;
    Ent80055F08 *end;

    if (D_80139B30 == 1) {
        func_80056044();
        ent = D_801D40DC;
        end = ent + 32;
        for (; ent < end; ent++) {
            if (ent->id != -1 && ent->state == 2) {
                D_8013A24C[ent->handler_index](ent);
            }
        }
        func_80055FD4();
    }
}
