#include <game/palette_animation.h>
extern void func_0202cc58(const void *, void *, u32);
extern void func_0202cd2c(const void *, void *, u32);

/* A keyframe stores a value, interpolation duration, and total segment time.
   Durations are shifted to the caller's eight-fractional-bit time scale. */
const s16 *GamePaletteAnimation_SelectKeyframes(const s16 *command, int *time, int *duration, int *from, int *to)
{
    int remaining = *time;
    const s16 *limit = command + 3 * command[0] - 2;
    ++command;
    while (1) {
        if (remaining < command[1] << 8) {
            *time = remaining;
            *duration = command[1] << 8;
            *from = command[0];
            *to = command[3];
            return limit;
        }
        remaining -= command[2] << 8;
        if (remaining < 0 || command == limit) {
            *time = 1;
            *duration = 1;
            *from = command[0];
            *to = command[3];
            return limit;
        }
        command += 3;
    }
}

const s16 *GamePaletteAnimation_Crossfade(const s16 *command, int time, u16 *destination,
                        int count, const u16 *palette)
{
    int duration, from, to;
    u16 colors[256];
    const s16 *end;
    const u16 *first;
    s16 amount;
    const u16 *second;
    end = GamePaletteAnimation_SelectKeyframes(command, &time, &duration, &from, &to);
    if (palette < colors)
        func_0202cd2c(palette, colors, 512);
    else
        func_0202cc58(palette, colors, 512);
    first = colors + from;
    second = colors + to;
    amount = 32 * time / duration;
    while (1) {
        if (--count < 0) return end;
        *destination++ = GameColor_Blend(*first++, *second++, amount);
    }
}
