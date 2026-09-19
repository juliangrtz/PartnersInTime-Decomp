extern "C" {

/*
 * Battle entry capture wait (overlay 2, 0x020757FC-0x02075818).
 *
 * Holds the entry until the field screen has been captured.
 */

#include <game/battle_entry.h>
#include <game/battle_frame.h>
#include <game/battle_context.h>
void func_ov002_02075780(void);
}

/* A separate frame boundary precedes expanding the captured image. */
void BattleEntry_WaitCapture(void)
{
    ((BattleFrameContextView *)gBattleContext)->update = func_ov002_02075780;
}
