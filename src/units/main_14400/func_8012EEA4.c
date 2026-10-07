#include "common.h"

typedef unsigned char u8;
typedef struct { s32 x; s32 y; } Dimensions;
typedef struct {
    s32 field_00;
    s32 field_04;
    s32 field_08;
    u8 field_0C[0x20];
} Frame;
typedef struct {
    s32 field_00;
    s32 field_04;
    u8 field_08;
    u8 field_09[3];
    Frame *field_0C;
    Dimensions *field_10;
} Sequence;
typedef struct {
    u8 field_00[0x10];
    void *field_10;
    s32 field_14;
    s32 field_18;
    s32 field_1C;
    Sequence *field_20;
    s32 field_24;
    s32 field_28;
    s32 field_2C;
    s32 field_30;
    s32 field_34;
    s32 field_38;
    s32 field_3C;
} Object;
extern void func_80027C20(void *, void *, s32);

void func_8012EEA4(Object *object, s32 action, void *data) {
    switch (action) {
        case 5: {
            Sequence *sequence;
            object->field_20 = data;
            object->field_3C = ((Sequence *)data)->field_00;
            object->field_30 = 0;
            sequence = object->field_20;
            switch (sequence->field_08) {
                case 0:
                    sequence->field_04 = (sequence->field_04 / 9) * 9;
                    object->field_24 = (object->field_20->field_10->x * (object->field_20->field_10->y << 1)) << 3;
                    if (object->field_20->field_0C) {
                        object->field_14 = object->field_20->field_0C->field_00;
                        object->field_18 = object->field_20->field_0C->field_04;
                        object->field_1C = object->field_20->field_0C->field_08;
                        func_80027C20(object->field_20->field_0C->field_0C, object->field_10, 0x20);
                    } else {
                        object->field_14 = object->field_18 = object->field_1C = 0;
                    }
                    break;
                case 1:
                    if (sequence->field_0C) {
                        object->field_14 = sequence->field_0C->field_00;
                        object->field_18 = object->field_20->field_0C->field_04;
                        object->field_1C = object->field_20->field_0C->field_08;
                    } else {
                        object->field_14 = object->field_18 = object->field_1C = 0;
                    }
                    break;
            }
            break;
        }
        case 4: {
            Sequence *previous = object->field_20;
            object->field_34 = 0;
            object->field_38 = 1;
            object->field_30 = 0;
            if (previous) {
                Sequence *current = object->field_20;
                u8 type;
                object->field_3C = previous->field_00;
                type = current->field_08;
                switch (type) {
                    case 0:
                        if (current->field_0C) {
                            object->field_1C = current->field_0C->field_08;
                        }
                        break;
                    case 1:
                        if (current->field_0C) {
                            object->field_1C = current->field_0C->field_08;
                        }
                        break;
                }
            }
            break;
        }
    }
}
