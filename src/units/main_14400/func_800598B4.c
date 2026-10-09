#include "common.h"
typedef unsigned char u8;
extern float D_80165438[4];
static inline float cell_center(u8 coordinate) { return (float)(coordinate * 32) + 16.0f; }
static inline float screen_coordinate(float coordinate) { return coordinate + 320.0f; }
void func_800598B4(u8 a, u8 b, u8 c, u8 d) {
    float x0, y0, x1, y1;
    x0 = screen_coordinate(cell_center(a));
    y0 = screen_coordinate(cell_center(c));
    x1 = screen_coordinate(cell_center(b));
    y1 = screen_coordinate(cell_center(d));
    D_80165438[0] = x0;
    D_80165438[1] = y0;
    D_80165438[2] = x1;
    D_80165438[3] = y1;
}
