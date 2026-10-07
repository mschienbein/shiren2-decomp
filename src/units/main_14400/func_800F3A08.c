#include "common.h"

typedef struct MonsterContainer MonsterContainer;

/* Monster-branch prefix; +0x8C is a container pointer, not the player's hunger. */
typedef struct MonsterContainerView {
    unsigned char pad0[0x8C];
    MonsterContainer *container;
} MonsterContainerView;

void func_800F3A08(MonsterContainerView *obj, MonsterContainer *container) {
    obj->container = container;
}
