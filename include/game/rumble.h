#ifndef PIT_GAME_RUMBLE_H
#define PIT_GAME_RUMBLE_H

#include <nitro/ctrdg.h>
#include <nitro/os_alarm.h>

/* The Rumble Pak is a cartridge in the GBA slot, so the driver has to cope with
   it being absent or pulled out mid-game; GameRumble_SetRemovedCallback is how
   a scene hears about that.

   A pattern is a sequence of up to six on/off pulse lengths in frames, played
   `repeats` times with `rest` frames between repetitions. */
typedef struct GameRumblePattern {
    u32 count;
    u32 rest;
    u32 on[6];
    u32 off[6];
    u32 repeats;
} GameRumblePattern;

/* The pattern currently playing, advanced from an OS alarm. */
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
/* Scenes install their own pattern bank and then play by index. */
void GameRumble_SetPatterns(const GameRumblePattern *patterns, u8 bank);
void GameRumble_InitControl(void);

#endif
