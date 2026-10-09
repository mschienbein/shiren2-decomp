#include "common.h"

typedef unsigned char u8;

/* Facing byte; the walk keeps its own copy (whole 1-byte object at 0x80147FF0, owned by
 * func_800D5A68's file). */
typedef struct {
    u8 value;
} Direction;

typedef struct {
    s32 x, y;
} Position;

typedef struct {
    Position position;
    Direction direction;
} Object;

extern Object *D_80147FE0;
extern Direction D_80147FF0;
Position D_80147FF4 = { 0, 0 }; /* walk start position */
extern u8 D_80147FFC;           /* walk started flag */

/* Start a walk for object: remember it, its facing and its start position; clear the started flag. */
void func_800D5A30(Object *object) {
    D_80147FE0 = object;
    D_80147FF0 = object->direction;
    D_80147FFC = 0;
    D_80147FF4 = object->position;
}
