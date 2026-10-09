#include "common.h"
typedef struct { short field_0, field_2; } Sprite;
extern s32 func_800751B4(s32 id, s32 flags);
extern void func_80075E5C(s32 id, s32 mode);
extern Sprite *func_8007946C(s32 type, s32 id);
extern s32 func_80076044(s32 id, s32 animation, s32 mode, s32 command, s32 flags);
void func_8008B8A8(s32 id, s32 flags, s32 mode, s32 animation) {
    Sprite *sprite;
    func_800751B4(id, flags);
    func_80075E5C(id, mode);
    sprite = func_8007946C(0, id);
    switch (mode) {
    case 2:
        if (sprite->field_2 == 0x17) {
            func_80076044(id, animation, 1, 0x24, 1);
            break;
        }
        func_80076044(id, animation, 1, 8, 3);
        break;
    case 1:
        if (sprite->field_2 == 0x17) {
            func_80076044(id, animation, 1, 0x22, 0);
            break;
        }
        func_80076044(id, animation, 1, 8, 3);
        break;
    case 0: case 3: case 4: case 5: case 6: case 7:
    default:
        func_80076044(id, animation, 1, 8, 3);
        break;
    }
}
