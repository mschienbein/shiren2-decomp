#include "common.h"
typedef struct { unsigned char field_0[0x14]; void *field_14; } State;
extern State *D_80148D84;
extern void func_800326CC(void *);
extern void func_800326AC(void *, void *);
void func_801307C4(void) { void *entry = D_80148D84->field_14; while (entry) { func_800326CC(entry); func_800326AC(entry, (unsigned char *)D_80148D84 + 4); entry = D_80148D84->field_14; } }
