/*
 * Palette animation (ARM9 resident, 0x0202D158-0x0202DAFC).
 *
 * Animated palettes: a track names a component of the animation table and an
 * effect to apply to it. Cycle rotates the entries, Reflect mirrors them, and
 * ApplyEffect writes the result into the live palette. The table accessors
 * resolve a track to its component, falling back to the default track when a
 * specific one is absent.
 */

#include <game/palette_animation.h>
typedef struct PaletteEffectTemplate {
    s16 command_offset, terminator;
    s16 mode, duration, start, end, period;
    s16 first_color, color_count, source_offset;
    union {
        struct {
            s16 keyframe_count, from, interpolation_duration, segment_duration, to, padding;
        } invert;
        struct {
            u16 color;
            s16 keyframe_count, from, interpolation_duration, segment_duration, to;
        } color;
    } keyframes;
} PaletteEffectTemplate;

typedef char PaletteEffectTemplate_SizeCheck[sizeof(PaletteEffectTemplate) == 32 ? 1 : -1];
extern PaletteEffectTemplate data_02059c38, data_02059c58;
extern int func_0202dca8(const s16 *, int, u16 *, int, const u16 *, int);

int GamePaletteAnimation_ApplyEffect(int mode, int color, int duration, int time,
    u16 *destination, int count, const u16 *source, int loop)
{
    int reverse = 0;
    PaletteEffectTemplate *table;
    s16 length;
    if (mode < 0) { mode = -mode; reverse = 1; }
    if (mode == 8) {
        table = &data_02059c38;
        table->mode = mode;
        if (reverse == 1) { table->keyframes.invert.from = 0; table->keyframes.invert.to = 31; }
        else { table->keyframes.invert.from = 31; table->keyframes.invert.to = 0; }
        length = duration;
        table->duration = length;
        table->end = length;
        table->period = length;
        table->keyframes.invert.interpolation_duration = length;
        table->keyframes.invert.segment_duration = length;
    } else {
        table = &data_02059c58;
        table->mode = mode;
        table->keyframes.color.color = color;
        if (reverse == 1) { table->keyframes.color.from = 0; table->keyframes.color.to = 31; }
        else { table->keyframes.color.from = 31; table->keyframes.color.to = 0; }
        length = duration;
        table->duration = length;
        table->end = length;
        table->period = length;
        table->keyframes.color.interpolation_duration = length;
        table->keyframes.color.segment_duration = length;
    }
    return func_0202dca8((const s16 *)table, time, destination, count, source, loop);
}

const s16 *GameAnimationTable_GetDefaultTrack(const s16 *table, int animation)
{
    int index = animation + 3;
    int offset;
    if (index < 2)
        return 0;
    if (table[0] <= index)
        index = 2;
    offset = table[index];
    if (!offset)
        offset = table[2];
    if (!offset) return 0;
    return table + offset;
}

const s16 *GameAnimationTable_GetTrack(const s16 *table, int animation)
{
    int index = animation + table[0];
    int offset;
    if (index < 2)
        return 0;
    if (table[1] <= index)
        return 0;
    offset = table[index];
    if (offset == 0)
        table = 0;
    else
        table += offset;
    return table;
}

const s16 *GameAnimationTable_GetComponent(const s16 *table, int component, int animation)
{
    int index;
    if (component < -1)
        return 0;
    if (component >= 4)
        return 0;
    if (animation < 0)
        return GameAnimationTable_GetDefaultTrack(table, component);
    index = animation * 4;
    if (component >= 0)
        index += component;
    return GameAnimationTable_GetTrack(table, index);
}

extern void MIi_CpuCopy16(const void *, void *, u32);

const s16 *GamePaletteAnimation_Cycle(const s16 *command, int time, u16 *destination,
                        int count, const u16 *palette)
{
    u16 colors[256];
    int length = command[1];
    int index;
    MIi_CpuCopy16(palette + command[0], colors, 2 * length);
    index = 0;
    while (1) {
        if (index == length) {
            index = 0;
            break;
        }
        time -= (command + index)[2] << 8;
        if (time < 0)
            break;
        ++index;
    }
    while (1) {
        if (--count < 0)
            return command + 2 + length;
        *destination++ = colors[index++];
        if (index == length)
            index = 0;
    }
}

const s16 *GamePaletteAnimation_Reflect(const s16 *command, int time, u16 *destination,
                        int count, const u16 *palette)
{
    /* Each turning point is emitted twice, matching the two-length period. */
    u16 colors[256];
    int length = command[1];
    int period = 2 * length;
    int index;
    MIi_CpuCopy16(palette + command[0], colors, 2 * length);
    index = 0;
    while (1) {
        if (index == period) {
            index = 0;
            break;
        }
        time -= (command + index)[2] << 8;
        if (time < 0)
            break;
        ++index;
    }
    if (length - 1 < index) {
        index = period - 1 - index;
        while (1) {
            if (--count < 0) return command + 2 + 2 * length;
            *destination++ = colors[index];
            if (index == 0) break;
            --index;
        }
    }
    while (1) {
        while (1) {
            if (--count < 0) return command + 2 + 2 * length;
            *destination++ = colors[index];
            if (index == length - 1) break;
            ++index;
        }
        while (1) {
            if (--count < 0) return command + 2 + 2 * length;
            *destination++ = colors[index];
            if (index == 0) break;
            --index;
        }
    }
}

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
