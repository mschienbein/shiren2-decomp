#include "common.h"

typedef unsigned char u8;
typedef struct Text80051860 { u8 state[0x10]; } Text80051860;
/* func_8009D570 places the text at 0x5C, drawing state at 0x6C,
 * and the next backing buffer at 0xFC, bounding the 0x7C text to 128 bytes. */
typedef struct LabelView {
    u8 pad_00[0x5C];
    Text80051860 text_5C;
    u8 drawing_6C[0x10];
    char string_7C[0x80];
} LabelView;
extern void func_800487EC(Text80051860 *text, s32 style, s32 flags, char *str);

void func_8009D5E4(LabelView *label)
{
    func_800487EC(&label->text_5C, 0, 0, label->string_7C);
}
