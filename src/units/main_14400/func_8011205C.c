#include "common.h"
typedef struct { unsigned char direction; } Direction;
/* Eight-byte map coordinate consumed by command 0xD4 (func_8004FA4C -> func_80050CEC). */
typedef struct { s32 x, y; } Position;
/* Partial context view: the origin coordinate lives at +0x2C. */
typedef struct { unsigned char pad00[0x2C]; Position origin_2C; } Context;
extern s32 func_80049CB4(s32 command, ...);
/* Both callers (func_8011E2B0, func_8011F720) supply a one-byte Direction aggregate;
 * this wrapper does not use it, but it stays part of the call contract. */
void func_8011205C(Context *context, Position *position, Direction direction) {
    func_80049CB4(0xD4, &context->origin_2C, position);
}
