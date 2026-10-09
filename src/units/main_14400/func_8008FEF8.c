#include "common.h"

typedef struct { s32 field00; s32 unknown04; } Entry;
typedef struct { s32 unknown00; s32 field04; unsigned char unknown08[0x14]; Entry *field1c; s32 unknown20; unsigned char field24; unsigned char unknown25[3]; float field28, field2c, field30, field34; unsigned short field38, field3a; unsigned char field3c, field3d, unknown3e[2], field40; } Source;
typedef struct { s32 unknown00[2]; void **field08; s32 unknown0c[4]; void **field1c; } Resources;
typedef struct { unsigned char unknown00[0x74]; Resources *field74; } Context;
typedef struct { unsigned char field00; unsigned char unknown01[0x47]; unsigned char field48, field49, field4a, field4b; s32 field4c, field50; float field54, field58, field5c, field60; unsigned char unknown64[0x13c]; s32 field1a0; unsigned char field1a4; unsigned char unknown1a5[0xf]; void *field1b4; } Target;
extern s32 func_8008D4B8(void *, void *, s32);
s32 func_8008FEF8(Source *source, Context *context, Target *target) {
    Target *output;
    s32 result = 0;
    /* ODD_C: group setup with its optional-resource exit; also shapes scheduling. */
    do {
        target->field1a0 = source->field04;
        if (!target->field00) {
            s32 mode = source->field24;
            output = target;
            switch (mode) {
            /* ODD_C: this mode byte takes 0/2/3/4; mode 2 leaves field48 unchanged.
             * The explicit case also orders GCC's compare tree as the ROM, which tests 2 first. */
            case 2: break;
            case 0: target->field48 = 0; break;
            case 3: target->field48 = mode; break;
            case 4: target->field48 = mode; break;
            }
            mode = source->field40;
            switch (mode) {
            case 0: output->field49 = 0; break;
            case 1: output->field49 = mode; break;
            case 2: output->field49 = mode; break;
            case 3: output->field49 = mode; break;
            }
            output->field4c = source->field38;
            output->field50 = source->field3a;
            output->field54 = source->field28;
            output->field58 = source->field2c;
            output->field5c = source->field30;
            output->field60 = source->field34;
            output->field4a = source->field3c;
            output->field4b = source->field3d;
        }
        if (!source->field1c) break;
        {
            s32 id = source->field1c[target->field1a4].field00;
            void *resource;
            if (!(id & 0x10000)) resource = context->field74->field08[id];
            else resource = context->field74->field1c[id & 0xffff];
            result = func_8008D4B8(target->field1b4, resource, 0);
        }
    } while (0);
    return result;
}
