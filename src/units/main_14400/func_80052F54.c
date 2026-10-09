#include "common.h"

/* Same 12-byte D_801397E0 record definition as func_80053034. */
typedef struct EntryState {
    s32 key_04;
    unsigned char state_08;
    unsigned char state_09;
    unsigned short unknown_0A;
} EntryState;
typedef struct Entry12 {
    short id_00;
    unsigned short unknown_02;
    EntryState state;
} Entry12;
Entry12 *func_80053034(short);
Entry12 *func_80052F54(short a) { return func_80053034(a); }
