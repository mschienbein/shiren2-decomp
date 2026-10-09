#include "common.h"

typedef unsigned char u8;

/* Short text field: up to `maxLength` characters plus terminator in `text`. */
typedef struct {
    u8 pad0[0x70];
    u8 text[8];
    s32 length;
    s32 maxLength;
} TextField;

void func_8009AE50(TextField *field, const u8 *src)
{
    s32 i = 0;
    u8 c;

    field->length = 0;
    if (src != 0) {
        for (; i < field->maxLength; i++) {
            field->length = i;
            c = *src;
            if (c == 0) {
                break;
            }
            field->text[i] = c;
            src++;
        }
    }
    for (; i <= field->maxLength; i++) {
        field->text[i] = 0;
    }
}
