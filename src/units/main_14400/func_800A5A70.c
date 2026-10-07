#include "common.h"

/* Only the observed byte at offset nine is modeled. Original object type,
 * field meaning, incoming signedness and concurrency protocol are unresolved.
 * Preserve its upper nibble and take the low nibble of the full input word.
 */
void func_800A5A70(void *object, u32 value)
{
    unsigned char *bytes = (unsigned char *)object;

    bytes[9] = (bytes[9] & 0xF0U) | (value & 0xFU);
}
