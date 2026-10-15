#include "map_rect.h"

typedef signed char s8;
typedef unsigned char u8;
typedef struct { s8 d; } Dir;
extern MapPos *func_800A25D8(MapPos *, MapPos *, Dir, s32);

static inline Dir *make_type(Dir *type, u8 value) {
    type->d = value;
    return type;
}

void func_800A30AC(MapRect *output, void *input, u8 index) {
    MapPos first;
    MapPos second;
    Dir first_type;
    Dir second_type;
    func_800A25D8(&first, input, *make_type(&first_type, 3), index);
    func_800A25D8(&second, input, *make_type(&second_type, 7), index);
    output->min = first;
    output->max = second;
}
