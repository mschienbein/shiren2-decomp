#include "common.h"
typedef unsigned char u8;
/* Partial view of the command-stream state object: only the flag byte at 0xD2. */
typedef struct { u8 pad0[0xD2]; u8 field_D2; } Obj8012981C;
/* Command handler from dispatch table D_801487D0: takes the state object and the byte
 * cursor just past the opcode, consumes no operand bytes and returns the cursor. */
u8 *func_8012981C(Obj8012981C *obj, u8 *cursor) { obj->field_D2 = 1; return cursor; }
