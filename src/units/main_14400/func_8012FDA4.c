#include "common.h"

/* Audio root and current audio context pointers (defined with func_8012FD60). */
extern void *D_80148D80;
extern void *D_80148D84;
extern void func_80130010(void);

void func_8012FDA4(void) {
    if (D_80148D80 != 0) {
        func_80130010();
        D_80148D80 = 0;
        D_80148D84 = 0;
    }
}
