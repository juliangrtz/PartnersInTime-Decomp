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
