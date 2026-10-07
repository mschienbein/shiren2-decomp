#include "common.h"
/* D_801487D0 handlers return the next byte-stream cursor.
 * The common dispatch supplies object; this handler intentionally ignores it. */
unsigned char *func_801298BC(void *object, unsigned char *cursor) {
    return cursor + 1;
}
