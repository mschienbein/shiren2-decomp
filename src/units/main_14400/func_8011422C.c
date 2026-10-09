#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

typedef struct {
    u8 pad0[0x18];
    void *owner;
} Link8011422C;

typedef struct {
    u8 pad0[0xC];
    Link8011422C link;
} Obj8011422C;

extern unsigned char *func_800D0054(Link8011422C *object, void *owner);

void *func_8011422C(Obj8011422C *obj) {
    Link8011422C *link = &obj->link;

    if (link->owner != obj) {
        func_800D0054(link, obj);
    }
    return link;
}
