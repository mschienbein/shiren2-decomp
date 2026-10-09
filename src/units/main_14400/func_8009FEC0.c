#include "common.h"
/* The resource destructor accesses the complete handle at subobject +0xC. */
typedef struct { unsigned char pad0[0x64]; unsigned char field64[0x10]; } Object;
extern void func_80048728(void *);
void func_8009FEC0(Object *object) { func_80048728(object->field64); }
