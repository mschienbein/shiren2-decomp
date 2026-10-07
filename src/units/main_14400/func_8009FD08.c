#include "common.h"
typedef struct { char pad[0x128]; s32 count; unsigned char ids[0x14]; } SA;
s32 func_8009FD08(SA *p, s32 id) {
    s32 i;
    for (i = 0; i < p->count; i++) {
        if (p->ids[i] == id) {
            return 1;
        }
    }
    return 0;
}
