#include "common.h"

void *D_80148D80 = 0;
void *D_80148D84 = 0;
void func_801303A0(void *);
void func_8012FD60(void *a, void *b){ if (D_80148D80 == 0) { D_80148D80 = a; if (D_80148D84 == 0) { D_80148D84 = a; func_801303A0(b);} } }
