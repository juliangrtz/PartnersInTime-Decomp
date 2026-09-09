extern "C" {
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
