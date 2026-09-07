#include <game/system.h>
#include <game/task.h>
#include <game/rumble.h>

extern u8 data_02059f50, data_0205a00c, data_02059f40, data_02059f4c, data_02059f44;
extern GameInput data_02059f68;
extern void func_020052b0(int frames, u32 parameter);

void GameSystem_UpdateVBlank(void)
{
    if (!data_02059f50) return;
    if (data_0205a00c) GameRumble_SkipPulse();
    if (!data_02059f40) {
        if (data_02059f4c) {
            if (!data_02059f44) func_020052b0(-1, 1);
        } else if (GameSystem_CheckResetKeys()) {
            if (data_02059f44) data_02059f4c = 1;
            else func_020052b0(-1, 1);
        }
    }
    GameIrqTask_UpdateAll();
}

int GameSystem_CheckResetKeys(void)
{
    u16 buttons = *(vu16 *)0x04000130;
    GameInput_Update(&data_02059f68, ((buttons | *(vu16 *)0x027fffa8) ^ 0x2fff) & 0x2fff);
    /* L + R + Start + Select, with no other buttons held. */
    if (data_02059f68.held == 0x30c && (data_02059f68.pressed & 0x30c)) return 1;
    return 0;
}
