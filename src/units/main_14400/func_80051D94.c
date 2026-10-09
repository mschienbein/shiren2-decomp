#include "common.h"
typedef struct { signed char x,y; } Pair;
typedef struct { s32 handle; Pair position; } Playback;
/* Complete 0xC-byte state: type at +0, handle at +4, position at +8. */
typedef struct { short type; Playback playback; } State;
extern State D_80161650;
extern unsigned char D_8016166C;
extern void *D_801D4D04;
extern char D_8014B844[];
extern void func_8005239C(void),func_800533DC(void);
extern int func_8012A58C(int,short);
extern void func_80052914(s32 index, void *dst, void *ranges);
extern s32 func_8012A534(s32 id, s32 value);
extern s32 func_80052AD8(void *resource),func_80129EE0(void *resource);
static inline Playback *playback(State *state) { return &state->playback; }
void func_80051D94(short type,Pair position) {
    void *saved = D_801D4D04;
    if (D_8016166C) {
        Playback *current;
        D_80161650.type = type;
        func_8005239C();
        func_800533DC();
        func_80052914((short)(type - 0x54), D_801D4D04, D_8014B844);
        if ((unsigned short)(type - 0x56) < 2U)
            D_80161650.playback.handle = func_80052AD8(saved);
        else
            D_80161650.playback.handle = func_80129EE0(saved);
        current = playback(&D_80161650);
        func_8012A534(current->handle, (unsigned char)position.x);
        func_8012A58C(current->handle, (unsigned char)position.y);
        current->position = position;
    }
}
