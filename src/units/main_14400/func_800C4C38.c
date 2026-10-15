#include "common.h"
#include "item_records.h"

typedef struct { char pad[0x10]; Item *unk10; } S;
extern char *func_800AC9D8(Item *);
char *func_800C4C38(S *s) { return func_800AC9D8(s->unk10); }
