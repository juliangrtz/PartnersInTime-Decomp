#ifndef PIT_CREDITS_TEXT_INTERNAL_H
#define PIT_CREDITS_TEXT_INTERNAL_H
#include "frontend_scene_internal.h"
#include <game/overlay005_resource.h>

/* A 72-byte pooled element controls one strip of the scrolling credits text.
 * Its two scale ramps are staggered before the strip crosses between screens. */
typedef struct CreditsTextStripTask {
    u8 unknown_00[36];
    int first_delay;
    u8 text_slot, second_delay;
    u16 unknown_2a;
    u16 text_width;
    u8 unknown_2e[6];
    s32 scroll_y, first_scale, second_scale;
    u8 unknown_40[8];
} CreditsTextStripTask;
typedef char CreditsTextStripTask_SizeCheck[sizeof(CreditsTextStripTask) == 72 ? 1 : -1];

void CreditsTextStrip_Update(CreditsTextStripTask *task);
#endif
