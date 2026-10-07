#include "common.h"
typedef struct { unsigned char pad0[8]; void *field_8; } Obj8011B560;
extern void *func_800AC5B4(s32, s32);
extern Obj8011B560 *func_8011B560(Obj8011B560 *);
void *func_8011B598(void){ return func_8011B560(func_800AC5B4(0x10, 0)); }
