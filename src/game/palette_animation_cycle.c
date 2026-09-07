#include <game/palette_animation.h>
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
