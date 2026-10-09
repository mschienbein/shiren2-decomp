#include "common.h"
typedef struct { unsigned char field_00[0x1C]; void *field_1C; } Object;
extern char D_8015EF30[];
extern Object *func_80111E08(Object *, void *, void *, void *, unsigned char *);
static inline void *init_base(void *object, char *data, void *value) { func_80111E08(object, data, value, data, (unsigned char *)data + 8); return object; }
Object *func_8011E63C(Object *object, char *data, void *value) { Object *result = init_base(object, data, value); result->field_1C = D_8015EF30; return result; }
