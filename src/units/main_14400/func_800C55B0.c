#include "common.h"

/* Four unsigned word steps observed in the original routine. The callers
 * consume the stored word; the historical return type and API name are unknown.
 */
void func_800C55B0(u32 *word)
{
    u32 value0 = *word;
    u32 value1 = (value0 << 8) | (((value0 >> 10) ^ (value0 >> 23)) & 0xFF);
    u32 value2 = (value1 << 8) | (((value1 >> 10) ^ (value1 >> 23)) & 0xFF);
    u32 value3 = (value2 << 8) | (((value2 >> 10) ^ (value2 >> 23)) & 0xFF);

    *word = (value3 << 8) | (((value3 >> 10) ^ (value3 >> 23)) & 0xFF);
}
