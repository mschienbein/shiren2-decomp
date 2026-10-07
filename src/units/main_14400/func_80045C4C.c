#include "common.h"
extern void func_80052614(unsigned char value);
/* Void facade: forwards the low byte of the setting; no result is produced. */
void func_80045C4C(s32 value) { func_80052614(value & 0xFF); }
