#include "func_801EA810_13C730/types.h"
extern s32 func_8010B9F4(OvlItem *item);
extern s32 func_8010BA90(OvlItem *item, s16 difference);
extern void func_800AE6C4(OvlItem *item, u16 value);

void func_801EA810_13C730(OvlItem *item, s32 value)
{
    switch (item->field00) {
    case 3:
    case 4:
        {
            OvlMessage message;
            /* FAKEMATCH: redundant alias for the flag write, also passed to
             * the real message handler. Direct fields differ by 5 words;
             * a by-address setter differs by 3. This is also the construction
             * pattern in canonical func_800AB3F4. ROM 0x13C79C uses base+0x18. */
            OvlMessage *event = &message;
            func_8010BA90(item, value - func_8010B9F4(item));
            message.field00 = 0x19;
            event->field18 = value < 0;
            item->field08->method3C((u8 *)item + item->field08->delta38, event);
        }
        break;
    case 7:
        item->field0C.byte = value;
        break;
    case 9:
        {
            OvlCollection *collection = &item->field0C.collection;
            collection->field04->method1C((u8 *)collection + collection->field04->delta18, value);
        }
        break;
    case 5:
    case 11:
    case 12:
    case 13:
        func_800AE6C4(item, value);
        break;
    case 14:
    case 18:
        item->field0C.word = value;
        break;
    case 17:
        if ((u32)(value - 1) < 3) {
            u8 quantity;
            item->field0C.word = value;
            quantity = 1;
            if (value == 3) {
                quantity = 2;
            }
            item->field04 = quantity;
        }
        break;
    }
}
