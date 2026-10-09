#include "common.h"

/* Both alternatives are object pointers: func_800C4360 stores the owner at
 * +0x20 and initializes the replacement source at +0x3C. The selected source
 * is forwarded to the damage record constructor by func_801250B0. */
typedef struct {
    unsigned char pad_00[0x20];
    void *owner_20;
    unsigned char pad_24[0x18];
    void *source_3C;
} Path;
void *func_800C47A8(Path *path) {
    void *source = path->source_3C;
    if (source == 0) {
        source = path->owner_20;
    }
    return source;
}
