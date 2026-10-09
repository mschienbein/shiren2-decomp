#include "common.h"
typedef struct { char pad0[0x44]; s32 field44; char pad48[8]; } Record;
extern unsigned char D_801DE984[];
extern Record D_801DEA0C[2];
void func_8006EA40(unsigned char *arg0);
s32 func_80070598(void *object, void *value);
void func_8005CE68(void) { Record *entry = D_801DEA0C + 2; s32 i; func_8006EA40(D_801DE984); for (i = 1; i != -1; --i) { --entry; if (entry->field44 != -1) func_80070598(D_801DE984, entry); } }
