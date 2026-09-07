#ifndef PIT_GAME_RUMBLE_H
#define PIT_GAME_RUMBLE_H

#include <nitro/ctrdg.h>
#include <nitro/os_alarm.h>

typedef struct GameRumblePattern {
    u32 count;
    u32 rest;
    u32 on[6];
    u32 off[6];
    u32 repeats;
} GameRumblePattern;

typedef struct GameRumbleState {
    u32 step;
    u32 last_step;
    u32 rest;
    u32 on[6];
    u32 off[6];
    int active;
    u32 repeats;
    u32 completed;
    void (*removed)(void);
} GameRumbleState;

typedef char GameRumbleStateSizeCheck[(sizeof(GameRumbleState) == 76) ? 1 : -1];

int GameRumble_InitCartridge(void);
void GameRumble_Write(u16 value);
int GameRumble_IsPresent(void);
int GameRumble_Init(void);
void GameRumble_Start(const GameRumblePattern *pattern);
void GameRumble_Stop(void);
int GameRumble_IsActive(void);
void GameRumble_SkipPulse(void);
void GameRumble_SetRemovedCallback(void (*callback)(void));
void GameRumble_Alarm(void *argument);
int GameRumble_Removed(int event);
void GameRumble_ControlRemoved(void);
void GameRumble_UpdateControl(void);
void GameRumble_PlayTimed(int pattern, u32 frames);
void GameRumble_PlayRepeated(int pattern, u32 repeats);
void GameRumble_PlayPattern(const GameRumblePattern *pattern, u16 frames);
void GameRumble_SetPatterns(const GameRumblePattern *patterns, u8 bank);
void GameRumble_InitControl(void);

#endif
