#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct Unit8004D170 {
    u8 pad_00[0x2];
    s16 id_02;
    u8 pad_04[0x3E - 0x4];
    u8 state_3E;
    u8 pad_3F;
    u8 done_40;
} Unit8004D170;

typedef struct Task8008869C {
    u8 pad_00[0x4];
    u16 phase_04;
    u8 pad_06[0x2];
    u16 started_08;
    u8 pad_0A[0x14 - 0xA];
    s32 slot_14;
    u8 pad_18[0x4];
    s32 timer_1C;
} Task8008869C;

Unit8004D170 *func_8007946C(s32 side, s32 slot);
s32 func_80077BA4(s32 kind, s32 value);

void func_8008869C(Task8008869C *task) {
    Unit8004D170 *unit = func_8007946C(0, task->slot_14);

    if (task->started_08 == 0) {
        task->started_08++;
        unit->done_40 = 0;
        unit->state_3E = 0xE;
        func_80077BA4(unit->id_02, -1);
    }
    if (task->timer_1C-- == 0) {
        task->phase_04 = 4;
        func_80077BA4(unit->id_02, 0);
        unit->done_40 = 1;
    }
}
