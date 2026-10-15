#include "common.h"

typedef struct { unsigned char kind, variant, field_02, flags, row, field_05; unsigned short field_06; unsigned char mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
/* Observed unsigned byte view of existing storage; allocation ownership and
 * the higher-level meaning of bit 5 remain unresolved. This TU owns no data.
 */


int func_80042A50(void)
{
    return ((D_80142F18.flags >> 5) & 1) ^ 1;
}
