#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct OSMesgQueue OSMesgQueue;
typedef struct { s32 status; OSMesgQueue *queue; s32 channel; u8 id[32], label[32]; s32 version, dir_size, inode_table, minode_table, dir_table, inode_start_page; u8 banks, activebank; } OSPfs;
typedef struct { u16 period, remaining, accumulator; u8 state; } Slot;
extern OSPfs D_801E00EC[4];
extern OSMesgQueue D_801E028C;
extern s32 func_8002C820(OSPfs *pfs, s32 flag);
extern s32 func_8002C964(OSMesgQueue *queue, OSPfs *pfs, int channel);
s32 func_80131630(Slot *slot, u32 index) {
    s32 result = 0;
    switch (slot->state) {
    case 1:
        if (slot->accumulator) result = func_8002C820(&D_801E00EC[index], 0);
        else slot->state = 2;
        slot->accumulator--;
        break;
    /* ODD_C: state 2 (set by state 1 once the countdown ends) is idle; the label
     * shapes codegen: without it 117 words differ (488 vs 496 bytes). */
    case 2:
        break;
    case 3:
        if (slot->remaining) {
            u16 sum = slot->accumulator + slot->period;
            slot->accumulator = sum & 0xFF;
            if (sum >> 8) result = func_8002C820(&D_801E00EC[index], 1);
            else result = func_8002C820(&D_801E00EC[index], 0);
        } else {
            result = func_8002C820(&D_801E00EC[index], 0);
            slot->state = 1;
            slot->accumulator = 2;
        }
        slot->remaining--;
        break;
    case 4: {
        OSPfs *pfs = &D_801E00EC[index];
        result = func_8002C964(&D_801E028C, pfs, index);
        if (!result) func_8002C820(pfs, 0);
        slot->state = 1;
        slot->accumulator = 2;
        break;
    }
    }
    return result;
}
