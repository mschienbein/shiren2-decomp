#include "common.h"
typedef struct { unsigned char pad_00[8]; s32 field_08; } Object;
/* Operation 0x205 payload (handler func_801314A0): +4/+8 are the PFS game and
 * extension name pointers. The complete local payload also carries the
 * optional byte and buffer fields. */
typedef struct { Object *object; unsigned char *game_name; unsigned char *ext_name; unsigned char field_0C; void *field_10; } Request;
extern s32 func_80131F04(s32 command, Request *payload);
s32 func_80130EA0(Object *object, unsigned char *game_name, unsigned char *ext_name) {
    Request request;
    s32 result;
    request.game_name = game_name;
    request.object = object;
    request.ext_name = ext_name;
    result = func_80131F04(0x205, &request);
    object->field_08 = result;
    return result;
}
