#include "common.h"
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;

extern void func_800A9C8C(unsigned char first, unsigned char second);
void func_800AA014(unsigned char arg) { func_800A9C8C(D_80142F24.index, arg); }
