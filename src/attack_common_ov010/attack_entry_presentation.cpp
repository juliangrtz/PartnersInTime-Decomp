#include "attack_common_internal.h"

extern "C" {
void Overlay10Attack_ShowRating(const s16 *thresholds, const s8 (*offsets)[2], int offset_index,
                                BattleSceneObject *object, int failed, const int *effects)
{
    int rating;
    Overlay10Work *work = data_ov002_020c0710;
    if (failed)
        rating = 0;
    else {
        int score = work->score;
        if (score < thresholds[0])
            rating = 1;
        else if (score < thresholds[1])
            rating = 2;
        else
            rating = 3;
    }
    if (rating == 3)
        ((Overlay10RatingFlags *)(gBattleContext + 312))->perfect = 1;
    if (rating > work->rating) {
        BattleSound_Play(data_ov010_020c2aac[rating], 0, 0, 0);
        work->rating = rating;
    }
    BattleFeedback_SpawnAtObject(effects[rating], object, offsets[offset_index][0], offsets[offset_index][1], 12);
}
}

extern "C" {

#include <nitro/fx.h>
#include <game/battle_scene.h>
#include <game/battle_actor.h>
#include <game/battle_enemy_data.h>
#include <game/overlay010_enemy_state.h>

void Overlay10Motion_StopAll(BattleSceneObject *object)
{
    int i;
    for (i = 0; i <= 3; ++i)
        BattleSceneObject_StopMotionChannel(object, i, 0);
}

}

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
