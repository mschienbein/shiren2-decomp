#include "common.h"

struct ALPlayer_s;

/* Parameter-only synthesizer prefix; the only accessed member is head. */
struct Instance_80037320 {
    struct ALPlayer_s *head;
};

void func_80033070(struct Instance_80037320 *instance)
{
    instance->head = 0;
}
