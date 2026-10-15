/* Existing operand union extracted without token changes: OpenAI gpt-6-astra. */
#ifndef SCRIPT_OPERAND_H
#define SCRIPT_OPERAND_H
#include "common.h"

typedef union Operand {
    s32 word;
    unsigned short half[2];
    unsigned char byte[4];
} Operand;

#endif
