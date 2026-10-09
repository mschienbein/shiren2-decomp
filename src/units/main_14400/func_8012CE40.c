#include "common.h"
typedef struct Entry { s32 value_00; } Entry;
extern Entry *D_80148A84[7];
s32 func_8012CE40(void) {
    s32 maximum = 0;
    s32 i;
    for (i = 0; D_80148A84[i]; ++i) {
        s32 value = D_80148A84[i]->value_00;
        if (maximum < value) maximum = value;
    }
    return maximum;
}
