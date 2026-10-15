#include "func_801EEBE4_140B04/types.h"
extern S10Actor *D_801476B8;
extern s32 D_801F5D04_147C24;
extern s32 func_801E897C_13A89C(s32 id, float distance);
extern void func_801E8F20_13AE40(s32 frames);

void func_801EEBE4_140B04(S10Actor *actor, s32 frames)
{
    s32 amount = 0;
    if ((actor->flags1E >> 6) & 1) {
        S10ActorComponent *component = actor ? &((S10ActorA *)actor)->component78 : 0;
        amount = component->field2C;
    }
    if ((actor->flags1E >> 3) & 1) {
        S10ActorComponent *component = actor ? &((S10ActorB *)actor)->component84 : 0;
        amount = component->field2C;
    }
    if (amount && D_801F5D04_147C24 == 1 &&
        func_801E897C_13A89C(D_801476B8->field08, (float)(amount * 2))) {
        func_801E8F20_13AE40(frames + 10);
    }
}
