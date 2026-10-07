#include "common.h"

/* Opaque owner/configuration views; only pointer relay is used here. */
struct Instance_80037320;
struct Config_80033080;

/* Partial link view. The anchor's next word alone is accessed. */
typedef struct Link_800326AC {
    struct Link_800326AC *next;
    struct Link_800326AC *previous;
} Link_800326AC;

extern struct Instance_80037320 *D_80037320;
extern void func_80033070(struct Instance_80037320 *instance);
extern void func_80033080(struct Instance_80037320 *instance,
                          struct Config_80033080 *config);

/* Void results are working side-effect views, not historical API recovery. */
void func_80032650(struct Instance_80037320 *instance,
                   struct Config_80033080 *config)
{
    if (D_80037320 == 0) {
        D_80037320 = instance;
        func_80033080(instance, config);
    }
}

void func_8003267C(struct Instance_80037320 *instance)
{
    if (D_80037320 != 0) {
        func_80033070(instance);
        D_80037320 = 0;
    }
}

void func_800326AC(Link_800326AC *node, Link_800326AC *anchor)
{
    Link_800326AC *next = anchor->next;

    node->previous = anchor;
    node->next = next;
    if (anchor->next != 0) {
        anchor->next->previous = node;
    }
    anchor->next = node;
}

void func_800326CC(Link_800326AC *node)
{
    if (node->next != 0) {
        node->next->previous = node->previous;
    }
    if (node->previous != 0) {
        node->previous->next = node->next;
    }
}
