#include "map_rect.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

s32 func_800A32D8(const MapRect *a, const MapRect *b) {
    MapRect buf;

    func_800A324C(&buf, a, b);
    return func_800A34D0(&buf);
}
