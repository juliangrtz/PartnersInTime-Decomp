/* Reset scale, rotation and offsets while preserving each entry's flags. */
#include "battle_oam_internal.h"

void BattleOam_InitializeTransforms(BattleOamTransform *transform, int count)
{
    int i;

    for (i = 0; i < count; ++i, ++transform) {
        transform->scale_pair = 0x01000100; /* Both Q8 scales are one. */
        transform->rotation_offsets = 0;
    }
}
