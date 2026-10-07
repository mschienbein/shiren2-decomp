#include "common.h"
typedef struct { unsigned short field_00; void *field_04; } Object;
extern char D_80157FA8[], D_80158248[];
static inline void init_base(Object *object) { object->field_04 = D_80157FA8; object->field_00 = 0x37; }
Object *func_800DA2C0(Object *object) { init_base(object); object->field_04 = D_80158248; return object; }
