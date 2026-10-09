#include "common.h"
/* The embedded collection occupies 0xCC..0xE3, including its owner and text ID. */
typedef struct { unsigned char field_00[0xCC]; unsigned char field_CC[0x18]; } Object;
extern void func_800CF834(void *);
void func_800EC848(Object *object) { func_800CF834(object->field_CC); }
