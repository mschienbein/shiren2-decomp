#include "common.h"
typedef struct { unsigned char pad_00[0xC]; unsigned char field_0C; } Object;
s32 func_80128AF4(Object *object) {
    switch (object->field_0C & 8) {
    case 0: return 0;
    default: return 1;
    }
}
