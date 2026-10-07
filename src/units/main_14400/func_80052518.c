#include "common.h"
typedef struct { signed char first, second; } Pair;
typedef struct { s32 field0; Pair field4; } State;
extern State D_80161654;
extern unsigned char D_8016166C, D_8016166E;
extern s32 func_8012A534(s32 id, s32 value);
extern void func_8012A58C(s32, unsigned char);
void func_80052518(s32 id, Pair value) {
    if (!D_8016166E) {
        if (id == D_80161654.field0) {
            if (!D_8016166C) return;
            D_80161654.field4 = value;
        }
        func_8012A534(id, (unsigned char)value.first);
        func_8012A58C(id, value.second);
    }
}
