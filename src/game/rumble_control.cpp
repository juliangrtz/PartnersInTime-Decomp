extern "C" {
#include <game/rumble.h>
#include <game/save_data.h>

typedef struct GameRumbleOptions {
    u8 previous[0x514];
    u8 reserved0 : 6, enabled : 1, reserved7 : 1;
} GameRumbleOptions;

extern u8 data_0205a000, data_0205a004, data_0205a008, data_0205a00c;
extern u16 data_0205a010, data_0205a014;
extern const GameRumblePattern *data_0205a018;
extern void func_0203a850(void);

#define CONTROL_ENABLED (data_0205a00c && (!gSaveData || ((GameRumbleOptions *)gSaveData)->enabled))

void GameRumble_InitControl(void)
{
    data_0205a004 = 0;
    data_0205a018 = 0;
    data_0205a000 = 0;
    func_0203a850();
    OS_InitAlarm();
    data_0205a00c = GameRumble_Init() != 0;
    if (data_0205a00c) GameRumble_SetRemovedCallback(GameRumble_ControlRemoved);
}

void GameRumble_SetPatterns(const GameRumblePattern *patterns, u8 bank)
{
    data_0205a018 = patterns;
    data_0205a000 = bank;
}

void GameRumble_PlayPattern(const GameRumblePattern *pattern, u16 frames)
{
    if (CONTROL_ENABLED) {
        GameRumble_Start(pattern);
        data_0205a014 = frames;
    }
}

void GameRumble_PlayRepeated(int index, u32 repeats)
{
    if (CONTROL_ENABLED) {
        GameRumblePattern pattern = data_0205a018[index];
        if (repeats) pattern.repeats = repeats;
        GameRumble_PlayPattern(&pattern, 0);
    }
}

void GameRumble_PlayTimed(int index, u32 frames)
{
    if (CONTROL_ENABLED) {
        GameRumblePattern pattern = data_0205a018[index];
        pattern.repeats = 0;
        GameRumble_PlayPattern(&pattern, (u16)frames);
    }
}

void GameRumble_UpdateControl(void)
{
    if (CONTROL_ENABLED) {
        int previous = data_0205a008;
        data_0205a008 = GameRumble_IsActive() != 0;
        if (data_0205a008) {
            if (!previous) data_0205a010 = 0;
            ++data_0205a010;
            if ((data_0205a014 && data_0205a010 >= data_0205a014) || data_0205a010 >= 600) {
                if (data_0205a00c) GameRumble_Stop();
            }
        }
    } else data_0205a008 = 0;
}

void GameRumble_ControlRemoved(void)
{
    data_0205a00c = 0;
    data_0205a004 = 1;
}
}
