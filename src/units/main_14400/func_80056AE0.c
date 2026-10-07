#include "common.h"

typedef short s16;
typedef struct { char pad[0x50]; s16 f50; char pad52[4]; s16 f56; char pad58[2]; s16 f5A; } S;
void func_80056AE0(S *s) {
    if (s->f56 != -1) {
        if (s->f56 <= 0) {
            s->f50 = 4;
            s->f5A = 0;
        } else {
            s->f56--;
            s->f5A++;
        }
    }
}
