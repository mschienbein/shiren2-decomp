#ifndef SHIREN2_MAP_RECT_PRESETS_H
#define SHIREN2_MAP_RECT_PRESETS_H

#include "common.h"

/* Plain coordinate records, not live MapRect/MapPos objects. Geometry consumers
 * copy these named scalar fields into actual MapRect objects before use. */
typedef struct MapRectPreset {
    s32 min_x;
    s32 min_y;
    s32 max_x;
    s32 max_y;
} MapRectPreset;

typedef struct MapRectPresets {
    MapRectPreset entries[6];
} MapRectPresets;

#endif
