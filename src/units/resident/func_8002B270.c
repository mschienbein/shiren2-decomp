#include "common.h"

typedef struct {
    long quot;
    long rem;
} ldiv_t;

typedef struct {
    long long quot;
    long long rem;
} lldiv_t;

/* ldiv */
ldiv_t func_8002B270(long num, long denom)
{
    ldiv_t ret;

    ret.quot = num / denom;
    ret.rem = num - denom * ret.quot;
    if (ret.quot < 0 && ret.rem > 0) {
        ret.quot++;
        ret.rem -= denom;
    }
    return ret;
}

/* libgcc __divdi3; the accepted build names it only by address */
extern long long func_80035F50(long long num, long long denom);

/* lldiv */
lldiv_t func_8002B2F4(long long num, long long denom)
{
    lldiv_t ret;

    ret.quot = func_80035F50(num, denom);
    ret.rem = num - denom * ret.quot;
    if (ret.quot < 0 && ret.rem > 0) {
        ret.quot++;
        ret.rem -= denom;
    }
    return ret;
}
