#include "common.h"
/* The renderer descriptors are 32 complete 0x60-byte records. */
typedef struct {
    unsigned char descriptor_00[0x44]; s32 id_44; unsigned char pad_48[0x18];
} Record80055758;
extern Record80055758 D_801D40DC[32];
extern s32 func_80070598(void *object, void *value);
void func_80055758(void *object) {
    s32 i = 0;
    s32 empty_id = -1;
    Record80055758 *record = D_801D40DC;
    for (; i < 32; i++) {
        if (D_801D40DC[i].id_44 != empty_id) {
            func_80070598(object, record);
        }
        record++;
    }
}
