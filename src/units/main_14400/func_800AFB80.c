#include "common.h"

extern void *const D_80153AE4[];
extern s32 func_800AFD08(void *table, void *obj);
void *func_800AFB80(void *obj) { s32 i=0; for (;;) { void *entry; if (i>=7) break; entry=D_80153AE4[i]; if ((unsigned char)func_800AFD08(entry,obj)!=255) return entry; i++; } return 0; }
