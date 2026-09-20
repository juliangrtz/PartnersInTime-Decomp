/* Prepare the Chomp pursuit, overlay 18, 0x020C42EC-0x020C43DC. */
#include "pocket_chomp_internal.h"
extern "C" void func_ov018_020c4174(PocketChomp *, int, PocketChompAdultMotion *, int);
extern "C" void PocketChomp_PreparePursuit(PocketChomp *center, s8 direction, int delay)
{
    PocketChompAttackWorkPrefix *work = data_ov002_020c0710;
    int target_id = work->target_id;
    /* The original performs this lookup even though its result is unused. */
    BattleSceneObject_GetById(target_id);
    PocketChomp_PositionAtSide(center, target_id, direction);
    PocketChomp_SetAnimation(center, 2, direction);
    center->alternate_bounce_timer = 0;
    center->bits.variant = 0;
    center->object->render_state = 32767;
    center->height_q12 = 0;
    center->velocity_q12 = 0;
    center->bits.sound_played = 0;
    center->bits.unknown_07 = 0;
    if (delay) {
        center->bits.launch_pending = 1;
        center->timer = delay;
        center->bits.phase = 0;
    } else {
        func_ov018_020c4174(center, direction, work->active_adult, work->contact_offset);
        center->bits.phase = 1;
    }
}
