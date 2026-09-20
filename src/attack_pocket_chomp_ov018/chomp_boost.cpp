/* Support swing boost, overlay 18, 0x020C3C34-0x020C3CFC. */
#include "pocket_chomp_internal.h"
extern "C" void func_ov018_020c3b28(PocketChomp *, int);
extern "C" void PocketChomp_Boost(PocketChomp *center)
{
    switch (center->bits.phase) {
    case 1:
    case 2:
        PocketChompAttackWorkPrefix *work = data_ov002_020c0710;
        center->alternate_bounce_timer = 60;
        center->bits.variant = 1;
        center->object->render_state = 31;
        PocketChomp_SetAnimation(center, 4, center->bits.direction);
        /* Preserve the native double-precision scaling and final truncation. */
        int speed = Overlay18Attack_GetTravelSpeedQ8(work->progress);
        func_ov018_020c3b28(center, (int)(384.0 * speed / 256.0));
        break;
    }
}
