#include "field_views_800D0054.h"

unsigned char *func_800D0054(FieldView800D0054 *object, void *owner) {
    unsigned char *pointer = object->bytes_10;

    object->owner_18 = owner;
    object->pointer_08 = pointer;
    return pointer;
}
