#include "common.h"

typedef unsigned char u8;
typedef float f32;

typedef struct Stream Stream;

typedef struct {
    f32 x;
    f32 y;
} Point;

/* 'FUNC' resource (see func_80091280): point count at +0x1C, point table at +0x20. */
typedef struct {
    u8 pad0[0x1C];
    u32 count;
    Point *points;
} Path;

f32 func_8008E178(Stream *stream);
void *func_80091450(u32 size);
void func_80091544(void *item);

/* Reads the path's points from the stream; returns the bytes read, or -1 (freeing the
 * table) when the allocation fails. */
s32 func_80091198(Stream *stream, Path *path)
{
    s32 failed = 0;
    s32 bytes = 0;
    u32 i;

    /* ODD_C: groups allocation and read; a failed allocation breaks to the shared cleanup below.
     * Also shapes scheduling: the if/else and goto-done forms each differ in 14 words. */
    do {
        path->points = func_80091450(path->count * 8);
        if (path->points == 0) {
            failed = -1;
            break;
        }
        for (i = 0; i < path->count; i++) {
            path->points[i].x = func_8008E178(stream);
            bytes += 4;
            path->points[i].y = func_8008E178(stream);
            bytes += 4;
        }
    } while (0);
    if (failed) {
        bytes = -1;
        if (path->points != 0) {
            func_80091544(path->points);
            path->points = 0;
        }
    }
    return bytes;
}
