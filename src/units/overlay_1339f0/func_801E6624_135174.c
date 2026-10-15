#include "func_801E6624_135174/types.h"

extern void func_80048240(u16 message, ...);
extern void func_80048464(void);
extern void func_80048380(void);
extern void func_80049BF0(s32 mode);
extern s32 func_80049CB4(s32 action, ...);
extern void func_801E63C4_134F14(void);
extern s32 func_801E65C0_135110(u8 first, u8 second);
extern s32 func_801E65E0_135130(u8 first, u8 second);
extern s32 func_801E6604_135154(u8 first, u8 second);
extern s32 func_801E6404_134F54(u8 slot);
extern void *func_800CA760(OvlRecord *record);
extern s32 func_800CA76C(OvlRecord *record, u8 kind);
extern void func_800CAAF0(OvlRecord *record);
extern void func_800CA80C(OvlRecord *record);

/* The caller owns eight s32 results. Operation/source/destination are words
 * 1/2/3; lbu at +0xB/+0xF narrows the corresponding slot words. */
void func_801E6624_135174(const s32 *request) {
    OvlRecord record;
    s32 result;
    switch (request[1]) {
    case 1:
        func_80048240(0x505);
        func_80048240(0x506);
        func_80048464();
        result = func_801E65C0_135110((u8)request[2], (u8)(request[3] - 2));
        break;
    case 2:
        func_80048240(0x505);
        func_80048240(0x506);
        func_80048464();
        if (request[3] < 2) {
            result = func_801E65E0_135130((u8)request[3], (u8)request[2]);
            if (result == 0x515) {
                s32 state;
                func_800CA760(&record);
                state = func_800CA76C(&record, (u8)request[3]);
                switch (state) {
                case 3:
                    func_800CAAF0(&record);
                    break;
                case 1:
                case 2:
                    result = 0x50C;
                    break;
                default:
                    result = 0x513;
                    break;
                }
                func_800CA80C(&record);
            }
        } else {
            result = func_801E6604_135154((u8)request[2], (u8)(request[3] - 2));
        }
        break;
    case 3:
        if (request[3] == 0) {
            goto finished;
        }
        result = func_801E6404_134F54((u8)request[2]);
        break;
    default:
        goto finished;
    }
    if (result != -10) {
        func_80048380();
        func_80048240((u16)result);
        func_80049BF0(0);
        func_80049CB4(2);
    }
finished:
    func_80048380();
    func_801E63C4_134F14();
}
