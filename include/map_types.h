#ifndef SHIREN2_MAP_TYPES_H
#define SHIREN2_MAP_TYPES_H

/*
 * Shared C++ map value types of the main image (one definition for every C++ TU that
 * uses them; see docs/OMP_WORKFLOW.md, C++ source rules).
 */

#include "common.h"

/*
 * Map cell coordinate: s32 x at +0, s32 y at +4 (8 bytes). The user-declared member-wise
 * copy constructor is part of the type: g++ 2.8.1 keeps copies in memory, copies them field
 * by field, and returns a MapPos by value through the hidden result pointer. Plain
 * assignment stays the implicit bitwise operator=. The empty default constructor makes g++
 * emit its (empty) element loop when it constructs a MapPos array.
 */
struct MapPos {
    s32 x;
    s32 y;
    MapPos() {}
    MapPos(s32 px, s32 py) : x(px), y(py) {}
    MapPos(const MapPos &other) : x(other.x), y(other.y) {}
    void set(s32 px, s32 py) { x = px; y = py; }
    s32 isSet() { return y | x; }
    MapPos transposed() const { return MapPos(y, x); }
};

/*
 * Eight-way direction byte (0..7) at +0 (1 byte), unsigned: func_800A7204 reads it with
 * lbu and does not match with a signed byte; no user's ROM load is a signed lb. Passed by
 * value left-justified in an argument register. The converting constructor masks to 0..7.
 */
struct MapDir {
    unsigned char value;
    MapDir() {}
    MapDir(s32 d) : value(d & 7) {}
    MapDir turned(s32 steps) const { return MapDir(value + steps); }
};

#endif
