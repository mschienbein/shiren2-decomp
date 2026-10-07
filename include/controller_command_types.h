#ifndef SHIREN2_CONTROLLER_COMMAND_TYPES_H
#define SHIREN2_CONTROLLER_COMMAND_TYPES_H

typedef unsigned char u8;
typedef unsigned long u32;
typedef signed int s32;

typedef struct {
    u8 dummy;
    u8 transmit_count;
    u8 receive_count;
    u8 command;
    u8 type_low;
    u8 type_high;
    u8 status;
    u8 end;
} ControllerCommand;

/* Observed existing 60-byte word area and its four-byte control word.
 * This declaration defines no storage or historical SDK type. */
typedef struct {
    u32 words[15];
    u32 command_status;
} ProbePifRam __attribute__((aligned(16)));

extern ProbePifRam D_80040FD0;
extern u8 D_80039008;

typedef char command_size_check[(sizeof(ControllerCommand) == 8) ? 1 : -1];
typedef char command_alignment_check[(__alignof__(ControllerCommand) == 1) ? 1 : -1];
typedef char pif_size_check[(sizeof(ProbePifRam) == 64) ? 1 : -1];
typedef char pif_alignment_check[(__alignof__(ProbePifRam) == 16) ? 1 : -1];
typedef char word_size_check[(sizeof(u32) == 4) ? 1 : -1];

void func_80027B60(u8 command);

#endif
