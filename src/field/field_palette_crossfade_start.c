#include <game/field_palette_crossfade.h>
void FieldPaletteCrossfade_Start(FieldAreaContext *field, int region, int source, int target, int duration)
{
    field->palette_crossfade->flags.active = 1;
    field->palette_crossfade->flags.paused = 0;
    field->palette_crossfade->flags.captured = 0;
    field->palette_crossfade->flags.region = region;
    field->palette_crossfade->flags.source_bank = source;
    field->palette_crossfade->flags.target_bank = target;
    field->palette_crossfade->duration = duration;
    field->palette_crossfade->time = 0;
}
