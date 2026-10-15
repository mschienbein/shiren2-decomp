#ifndef S11_REPAIRED_RECORD_VIEWS_H
#define S11_REPAIRED_RECORD_VIEWS_H
#include "common.h"

/* Real 16-byte rectangle at component+0x1C. 800E6170 forwards its address
 * to 800A31C8, whose loads at 800A31CC/31DC/31F0/3200 read all four words.
 * Setters: x0 sw 801F2B2C, y0 sw 801F2B28, x1 sw 801F2B44, y1 sw 801F2B38.
 * Same x0,y0,x1,y1 view as canonical func_800A31C8.c. */
typedef struct EventRect {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
} EventRect;

/* Event/spawn record +2 is both a packed halfword (lhu at 801F1AB4) and
 * the y/x byte pair (lbu at 801F1690/801F1698; stores 801F363C/801F3644).
 * The big-endian target places y in the halfword's high byte. This is an
 * actual overlapping storage view, not a change to record size or stride. */
typedef union EventCoordinates {
    unsigned short packed;
    struct {
        unsigned char y;
        unsigned char x;
    } bytes;
} EventCoordinates;

#endif
