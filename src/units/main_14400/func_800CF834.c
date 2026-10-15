#include "common.h"
#include "collection_iterator.h"
typedef unsigned char u8;
/* Each iterator includes CEBA0's current-item pointer at +0xC. */
extern void func_800ACD34(void *object);
extern void *func_8011422C(u8 *object);
void func_800CF834(void *collection) {
    CollectionIterator outer;
    CollectionIterator inner;
    u8 *object;
    func_800CEB20(&outer, collection);
    while (func_800CEBA0(&outer)) {
        object = func_800CEC68(&outer);
        func_800ACD34(object);
        if (*object == 9) {
            func_800CEB20(&inner, func_8011422C(object));
            while (func_800CEBA0(&inner)) func_800ACD34(func_800CEC68(&inner));
        }
    }
}
