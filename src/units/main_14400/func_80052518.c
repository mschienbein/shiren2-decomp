#include "common.h"
typedef struct { signed char x, y; } Pair;
typedef struct { s32 handle; Pair position; } Playback;
typedef struct { short type; Playback playback; } State;
extern State D_80161650;
extern unsigned char D_8016166C, D_8016166E;
extern s32 func_8012A534(s32 id, s32 value);
extern int func_8012A58C(int, short);
void func_80052518(s32 id, Pair value) {
    if (!D_8016166E) {
        if (id == D_80161650.playback.handle) {
            if (!D_8016166C) return;
            D_80161650.playback.position = value;
        }
        func_8012A534(id, (unsigned char)value.x);
        func_8012A58C(id, (unsigned char)value.y);
    }
}
