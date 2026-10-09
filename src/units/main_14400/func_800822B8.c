#include "common.h"
/* Ten original records, 0x2C bytes each, rooted at their true array base. */
typedef struct {
    unsigned short field00, active, field04, originX, originY, width, height, field0E, dirty;
    unsigned char pad12[0x1A];
} Record;
extern Record D_801A9080[10];
extern s32 D_8013E8E0;
extern void func_80083568(void);
void func_800822B8(u32 index, s32 width, s32 height) {
    if (index < 10 && D_801A9080[index].active != 0 &&
        D_801A9080[index].originX + width < 39 && D_801A9080[index].originY + height < 29) {
        if (width < D_801A9080[index].width || height < D_801A9080[index].height) func_80083568();
        D_801A9080[index].width = width;
        D_801A9080[index].height = height;
        D_801A9080[index].dirty = 1;
        D_8013E8E0 = 1;
    }
}
