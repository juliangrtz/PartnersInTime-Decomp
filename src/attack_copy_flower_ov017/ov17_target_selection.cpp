/* Copy Flower target cycling (overlay 17, 0x020C2B8C-0x020C2C28).
 * Skip the prior target; prefer enemies without trait 1 on the first pass. */

#include <game/overlay017_attack.h>
#include <game/battle_feedback.h>
extern "C" int Overlay10Enemy_IsSelectable(u16);

extern "C" int Overlay17Attack_SelectNextTarget(u16 candidate)
{
    int previous = candidate;
    u16 wrapped = 0;
    do {
        candidate = (u16)(candidate + 1);
        if (candidate >= 66)
            candidate = 60;
        if (candidate == previous) {
            candidate = (u16)(candidate + 1);
            if (!wrapped)
                wrapped++;
            else
                return 0;
        }
    } while (!Overlay10Enemy_IsSelectable(candidate) ||
             (!wrapped && BattleActor_HasTrait1(candidate)));
    return candidate;
}
