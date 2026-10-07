#include "common.h"

typedef struct MonsterContainer MonsterContainer;

/* Monster-branch prefix; +0x8C is a container pointer, not the player's hunger. */
typedef struct MonsterContainerView {
    unsigned char pad0[0x8C];
    MonsterContainer *container;
} MonsterContainerView;

MonsterContainer *func_800F39E0(MonsterContainerView *obj) {
    return obj->container;
}
