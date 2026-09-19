/*
 * Low-HP warning stop (overlay 7, 0x02080C2C-0x02080C40).
 *
 * Stops the low-HP warning.
 */

#include "pause_scene_internal.h"
#include <game/pause_hp_warning.h>

extern "C" void PauseHpWarning_Stop(void)
{
    WORK.hp_warning_mode = PAUSE_HP_WARNING_DISABLED;
}
