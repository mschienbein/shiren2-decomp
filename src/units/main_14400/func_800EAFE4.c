#include "common.h"

typedef struct { unsigned char fields_00[5]; unsigned char field_05; } Item;
typedef struct { unsigned char fields_00[0xA0]; short field_A0; Item *(*field_A4)(void *, unsigned char); } VTable;
typedef struct { unsigned char fields_00[0x24]; VTable *field_24; } State;
extern s32 func_800E0F40(State *);
s32 func_800EAFE4(State *state) { unsigned char index = func_800E0F40(state); VTable *table = state->field_24; return table->field_A4((unsigned char *)state + table->field_A0, index)->field_05; }
