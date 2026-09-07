#include <game/palette_animation.h>

/* Apply each interpolated effect in the original overlap-safe copy direction.
   ARM compares the addresses as signed words. The two early-return loops also
   preserve the compiler's trailing, unreachable epilogue. */

const s16 *GamePaletteAnimation_Blend(const s16 *command, int time, u16 *destination,
                        int count, const u16 *palette)
{
    int duration, from, to;
    const u16 *source = palette + command[0];
    int tint = (u16)command[1];
    const s16 *end;
    s16 amount;
    end = GamePaletteAnimation_SelectKeyframes(command + 2, &time, &duration, &from, &to);
    amount = from + time * (to - from) / duration;
    if ((s32)destination < (s32)source) {
        while (1) {
            if (--count < 0) return end;
            *destination++ = GameColor_Blend(*source++, tint, amount);
        }
    } else {
        while (1) {
            if (--count < 0) return end;
            destination[count] = GameColor_Blend(source[count], tint, amount);
        }
    }
}

const s16 *GamePaletteAnimation_Add(const s16 *command, int time, u16 *destination,
                        int count, const u16 *palette)
{
    int duration, from, to;
    const u16 *source = palette + command[0];
    int tint = (u16)command[1];
    const s16 *end;
    s16 amount;
    end = GamePaletteAnimation_SelectKeyframes(command + 2, &time, &duration, &from, &to);
    amount = from + time * (to - from) / duration;
    if ((s32)destination < (s32)source) {
        while (1) {
            if (--count < 0) return end;
            *destination++ = GameColor_AddScaled(*source++, tint, amount);
        }
    } else {
        while (1) {
            if (--count < 0) return end;
            destination[count] = GameColor_AddScaled(source[count], tint, amount);
        }
    }
}

const s16 *GamePaletteAnimation_Subtract(const s16 *command, int time, u16 *destination,
                        int count, const u16 *palette)
{
    int duration, from, to;
    const u16 *source = palette + command[0];
    int tint = (u16)command[1];
    const s16 *end;
    s16 amount;
    end = GamePaletteAnimation_SelectKeyframes(command + 2, &time, &duration, &from, &to);
    amount = from + time * (to - from) / duration;
    if ((s32)destination < (s32)source) {
        while (1) {
            if (--count < 0) return end;
            *destination++ = GameColor_SubtractScaled(*source++, tint, amount);
        }
    } else {
        while (1) {
            if (--count < 0) return end;
            destination[count] = GameColor_SubtractScaled(source[count], tint, amount);
        }
    }
}

const s16 *GamePaletteAnimation_Tint(const s16 *command, int time, u16 *destination,
                        int count, const u16 *palette)
{
    int duration, from, to;
    const u16 *source = palette + command[0];
    int tint = (u16)command[1];
    const s16 *end;
    s16 amount;
    end = GamePaletteAnimation_SelectKeyframes(command + 2, &time, &duration, &from, &to);
    amount = from + time * (to - from) / duration;
    if ((s32)destination < (s32)source) {
        while (1) {
            if (--count < 0) return end;
            *destination++ = GameColor_Tint(*source++, tint, amount);
        }
    } else {
        while (1) {
            if (--count < 0) return end;
            destination[count] = GameColor_Tint(source[count], tint, amount);
        }
    }
}

const s16 *GamePaletteAnimation_Invert(const s16 *command, int time, u16 *destination,
                        int count, const u16 *palette)
{
    int duration, from, to;
    const u16 *source = palette + command[0];
    const s16 *end;
    s16 amount;
    end = GamePaletteAnimation_SelectKeyframes(command + 1, &time, &duration, &from, &to);
    amount = from + time * (to - from) / duration;
    if ((s32)destination < (s32)source) {
        while (1) {
            if (--count < 0) return end;
            *destination++ = GameColor_Invert(*source++, amount);
        }
    } else {
        while (1) {
            if (--count < 0) return end;
            destination[count] = GameColor_Invert(source[count], amount);
        }
    }
}
