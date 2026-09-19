#ifndef PIT_GAME_INPUT_H
#define PIT_GAME_INPUT_H

#include <nitro.h>

/* One frame of button state, derived by GameInput_Update from the raw held
   mask. `repeated` reports auto-repeat: a button first repeats after
   `first_delay` frames and then every `repeat_delay` frames, tracked per
   button in `countdown`. */
typedef struct GameInput {
    u16 held;
    u16 pressed;
    u16 released;
    u16 repeated;
    u8 first_delay;
    u8 repeat_delay;
    u8 countdown[10];
} GameInput;

/* Frame-time instrumentation in scanlines: where the main loop and the IRQ
   handler started and ended, how long each took, and the running peaks with
   their hold timers. `pending_vblanks` counts VBlanks the main loop missed. */
typedef struct GameFrameTiming {
    u8 reserved[20];
    union {
        u16 flags;
        struct { u16 unknown_00_07 : 8, waiting_vblank : 1, unknown_09_15 : 7; } bits;
    };
    s16 frame_start;
    s16 frame_end;
    s16 frame_lines;
    s16 frame_vblanks;
    s16 pending_vblanks;
    s16 irq_start;
    s16 irq_end;
    s16 irq_lines;
    s16 frame_peak;
    s16 frame_peak_timeout;
    s16 frame_peak_hold;
    s16 irq_peak;
    s16 irq_peak_timeout;
    s16 irq_peak_hold;
} GameFrameTiming;

/* The resident per-frame workspace. Input sits at its start, which is why so
   much code reaches the pad through this block. */
typedef struct GameDisplayWork {
    GameInput input;
    u8 reserved[0x820];
    int display_mode;
} GameDisplayWork;

typedef char GameInputSizeCheck[(sizeof(GameInput) == 20) ? 1 : -1];

void GameInput_Read(void);
void GameInput_Update(GameInput *input, u16 held);
void GameFrame_WaitVBlank(void);
void GameDisplay_CopyOam(u32 dma);
void GameDisplay_ClearOam(void);
void GameDisplay_SetBackgroundColor(u16 main_color, u16 sub_color);
void GameFrame_VBlank(void);

#endif
