#include "common.h"

typedef s32 (*EntryPredicate)(void *);
typedef struct {
    void *list_00;
    void *entry_04;
    unsigned short message_08;
    EntryPredicate predicate_0C;
} EntryAction;

/* 800A1308 dereferences both objects and passes predicate_0C to 800CE46C. */
void func_800A12A4(void *out, void *list, void *entry, unsigned short message, EntryPredicate predicate)
{
    EntryAction *action = out;
    action->list_00 = list;
    action->entry_04 = entry;
    action->message_08 = message;
    action->predicate_0C = predicate;
}
