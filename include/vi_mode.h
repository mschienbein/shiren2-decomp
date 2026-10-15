#ifndef SHIREN2_VI_MODE_H
#define SHIREN2_VI_MODE_H

#include "common.h"

/* OSViMode register image, shared by the resident scheduler and game callers.
 * func_800341B0 reads ctrl at +4; func_80034410 reads the common words and
 * one of two 0x14-byte field records. PR/os_vi.h corroborates the byte type
 * at +0; bytes +1..+3 are natural alignment, not a u32 mode identifier. */
typedef struct {
    u32 ctrl, width, burst, vSync, hSync, leap, hStart, xScale, vCurrent;
} OSViCommonRegs;

typedef struct {
    u32 origin, yScale, vStart, vBurst, vIntr;
} OSViFieldRegs;

typedef struct {
    unsigned char type;
    OSViCommonRegs comRegs;
    OSViFieldRegs fldRegs[2];
} OSViMode;

/* Resident initialized table: ROM [0x128E0,0x13A60), 56 records of 0x50.
 * Entry 9 at 0x800377B0 is the menu's 640-wide NTSC HPF1 mode.
 * Storage remains with the resident data owner; this header allocates none.
 * Dynamic selectors still require a valid table index; no new guard is added. */
extern OSViMode D_800374E0[56];
extern void func_800341B0(OSViMode *mode); /* osViSetMode */

#endif
