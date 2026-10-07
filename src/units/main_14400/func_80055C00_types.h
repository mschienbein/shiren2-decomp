#ifndef CANDIDATE_80055C00_TYPES_H
#define CANDIDATE_80055C00_TYPES_H

typedef unsigned char u8;
typedef signed short s16;

/* Partial layout of one 0x60-byte record in the array at 0x801D40DC.
 * Names describe offsets only; original field names and meanings are unknown.
 * Neighboring routines read field_50 with lh and dispatch through 0x80139B1C.
 * The byte fields and counters are ordinary RAM, with no observed MMIO effect.
 */
typedef struct Record_801D40DC {
    u8 unresolved_00[0x14];
    u8 field_14;
    u8 field_15;
    u8 field_16;
    u8 field_17;
    u8 field_18;
    u8 field_19;
    u8 field_1A;
    u8 field_1B;
    u8 unresolved_1C[0x34];
    s16 field_50;
    s16 field_52;
    s16 field_54;
    s16 field_56;
    s16 field_58;
    s16 field_5A;
    s16 field_5C;
    s16 field_5E;
} Record_801D40DC;

#endif
