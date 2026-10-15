#ifndef SHIREN2_MAP_RECT_H
#define SHIREN2_MAP_RECT_H

#include "common.h"
#ifdef __cplusplus
#include "map_types.h"
#else
typedef struct {
    s32 x;
    s32 y;
} MapPos;
#endif

/* Two position subobjects, not an integer array. This describes the observed
 * geometry layout; it does not establish a foreign object's lifetime. */
typedef struct MapRect {
    MapPos min;
    MapPos max;
} MapRect;

#ifdef __cplusplus
extern "C" {
#endif
MapRect *func_800A2FD0(MapRect *output, void *input, unsigned char radius);
void func_800A30AC(MapRect *output, void *input, unsigned char radius);
MapRect *func_800A324C(MapRect *output, const MapRect *a, const MapRect *b);
s32 func_800A32D8(const MapRect *a, const MapRect *b);
s32 func_800A3308(const MapRect *a, const MapRect *b);
s32 func_800A3378(const MapRect *a, const MapRect *b);
void func_800A3448(MapRect *area, MapRect *bounds);
s32 func_800A34D0(const MapRect *bounds);
MapPos func_800A694C(void *center, MapRect *bounds, unsigned char filter, s32 check);
MapPos func_800A6B70(void *source, unsigned char filter, s32 check);
MapRect *func_800B1F58(MapRect *output);
MapRect func_800B3024(void *object);
MapRect func_800B3080(void *position);
void func_800B4A70(MapRect *bounds);
void func_800B6728(void *object, const MapRect *bounds);
void func_800B7948(void *object, s32 index, MapRect *bounds);
void func_800B7E9C(void *object, const MapRect *a, const MapRect *b);
#ifdef __cplusplus
}
#endif

#endif
