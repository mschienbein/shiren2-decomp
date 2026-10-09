#ifndef SHIREN2_CONTROLLER_RAM_H
#define SHIREN2_CONTROLLER_RAM_H

#include "common.h"
#include "controller_queue_view.h"

typedef unsigned short u16;
typedef unsigned char u8;

/* Observed 40-byte command view inside the existing 64-byte PIF buffer.
 * Queue ownership, original SDK names and complete type provenance are unknown. */
typedef struct {
    u8 dummy;
    u8 transmit_size;
    u8 receive_size;
    u8 command;
    u8 address_high;
    u8 address_low;
    u8 data[32];
    u8 checksum;
    u8 end;
} ResidentControllerRamPacket;

extern s32 D_80036F70;
extern u8 D_80039018;
/* One 64-byte PIF image; its final status word is not a separate object. */
typedef struct ResidentPifRam {
    u32 ramarray[15];
    u32 pifstatus;
} ResidentPifRam;
extern ResidentPifRam D_80041350;

extern void func_800322C4(void);
extern void func_80032330(void);
extern s32 func_80032500(s32 direction, void *buffer);
extern ControllerQueueS32 func_8002FEA0(ControllerQueueView *queue,
                                      ControllerQueueMessage *message,
                                      ControllerQueueS32 flags);
/* The original callee masks the low 16 bits. Historical parameter type unknown. */
extern u8 func_80027DB0(u32 block);
extern u8 func_80027E1C(const u8 *data);
extern s32 func_8002E720(void *queue, s32 channel);
/* The original copy helper returns destination; these callers discard it. */
extern void *func_800262C0(const void *source, void *destination, s32 count);

s32 func_80027330(void *queue, s32 channel, u16 block, u8 *destination);
s32 func_80027520(void *queue, s32 channel, u16 block, const u8 *data, s32 force);

typedef char controller_ram_word_check[(sizeof(u32) == 4) ? 1 : -1];
typedef char controller_ram_pointer_check[(sizeof(void *) == 4) ? 1 : -1];
typedef char controller_ram_packet_check[(sizeof(ResidentControllerRamPacket) == 40) ? 1 : -1];

#endif
