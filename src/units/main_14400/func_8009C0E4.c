#include "common.h"
typedef struct { unsigned char fields00[0x28]; short field28; void (*field2C)(void *); } Methods;
typedef struct { unsigned char fields00[0x4C]; Methods *field4C; unsigned char fields50[0x20]; unsigned char field70[8]; s32 field78; s32 field7C; } Editor;
s32 func_8009C0E4(Editor *editor) {
    s32 index;
    if (editor->field70[editor->field78] == 0) {
        if (editor->field78 <= 0) {
            return 0;
        }
        editor->field78--;
    }
    for (index = editor->field78; index < editor->field7C - 1; index++) {
        editor->field70[index] = editor->field70[index + 1];
    }
    editor->field70[editor->field7C - 1] = 0;
    editor->field4C->field2C((unsigned char *)editor + editor->field4C->field28);
    return 1;
}
