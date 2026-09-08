#include "attack_common_internal.h"

extern "C" {
void Overlay10Attack_BeginEntry(Overlay10ActionActor *user, void (*callback)(Overlay10ActionActor *))
{
    Overlay10Work *work = data_ov002_020c0710;
    int i = 0;
    work->phase = 0;
    work->timer2 = 0;
    work->timer = work->timer2;
    work->duration[0] = 0;
    work->duration[1] = 0;
    work->background_duration = 0;
    work->enabled = 1;
    work->prompt_animation = 20;
    work->background_phase = 0;
    work->mirror = 0;
    work->unknown_129 = 4;
    work->prompt_x = 32;
    work->prompt_y = 0;
    work->offset_x = 0;
    work->offset_y = 0;
    work->callback = callback;
    do {
        int formation;
        if (i == 1)
            BattleSceneObject_GetById(user->party.linked_object_id);
        if (i != 1)
            formation = user->party.formation_index & 1;
        else
            formation =
                ((BattlePartyActor *)BattleActor_GetById(user->party.linked_object_id))->formation_index & 1;
        s16 value = data_ov010_020c2ab4[formation][1];
        work->transitions[i].first = value;
        work->transitions[i].second = value;
        work->transitions[i].phase = 0;
        ++i;
    } while (i < 2);
    func_ov002_020722ac(user, func_ov010_020c1410);
}

void Overlay10Attack_MoveBackground()
{
    Overlay10Work *work = data_ov002_020c0710;
    BattleSceneObject *objects[3];
    objects[0] = BattleSceneObject_GetById(68);
    objects[1] = BattleSceneObject_GetById(69);
    if (work->background_duration) {
        s16 *delta = &work->background_x;
        for (int i = 0; i < 2; ++i) {
            BattleSceneObject_MoveBy(objects[i], 2, delta[0], delta[1], 0, work->background_duration);
        }
    }
}
}
