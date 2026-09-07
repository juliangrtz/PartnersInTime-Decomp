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
