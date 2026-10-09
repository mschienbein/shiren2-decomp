#include "common.h"
typedef struct { s32 x; s32 y; } Point;
typedef struct { char pad_00[0x78]; short adjust_78; short pad_7A; s32 (*method_7C)(void *, Point *); } VTable;
typedef struct { char pad_00[0x4C]; const VTable *field_4C; } Object;
/* Widget +0x8C is an opaque selection token, compared and saved without
 * dereferencing it. The default encodes the numeric +0x7C value as a token;
 * the item-dialog override func_80098964 instead returns an actual item pointer. */
void *func_80095C8C(Object *object, Point point) {
    return (void *)object->field_4C->method_7C((char *)object + object->field_4C->adjust_78, &point);
}
