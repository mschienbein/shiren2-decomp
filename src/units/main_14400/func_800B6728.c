#include "map_rect.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

void func_800B6590(void *self, MapPos *p, MapPos *q);
void func_800B6728(void *self, const MapRect *src) {
    MapPos p, q;
    p.x = src->min.x;
    p.y = src->min.y;
    q.x = src->max.x;
    q.y = src->max.y;
    func_800B6590(self, &p, &q);
}
