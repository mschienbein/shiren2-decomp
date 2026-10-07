#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

/* Parameter-only state prefix for the command handler's flag at +0xBA.
 * D_801487D0 handlers take the state and the command byte cursor and return
 * the next cursor; this one consumes no operand bytes.
 */
typedef struct { u8 pad[0xBA]; u8 unkBA; } S;
u8 *func_801295E0(S *arg0, u8 *cursor) { arg0->unkBA = 1; return cursor; }
