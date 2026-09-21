/*
 * Camera shake setup (overlay 0, 0x02072074-0x020721C0).
 * Saves the chosen axis origin and initializes the amplitude, per-update step
 * and half-cycle count. A rumble value of -1 selects a pattern from the period.
 */
extern "C" {
#include <nitro/fx.h>
#include <game/rumble.h>
}
#include <game/field_hud.h>

#define SHAKE_ABS(value) ((value) < 0 ? -(value) : (value))

extern "C" {

void FieldCamera_StartShake(void *context, int axis, fx32 amplitude, fx32 step,
                            int half_cycles, int rumble_pattern)
{
    FieldHudContext *field = (FieldHudContext *)context;
    field->shake.active = 1;
    field->shake.vertical = axis;
    field->shake_repetitions = half_cycles;
    field->shake_step = step;
    field->shake_amplitude = amplitude;
    field->shake_position = 0;
    /* The origin uses the full axis argument; the stored axis keeps one bit. */
    field->shake_origin = axis == 0 ? field->camera_x : field->camera_y;
    field->shake.rumble = rumble_pattern;
    if (field->shake.rumble == -1) {
        /* Native code evaluates FX_Div twice; preserve both hardware queries. */
        field->shake.rumble = SHAKE_ABS(FX_Div(amplitude, step)) >= 4096 ? 1 : 2;
        GameRumble_PlayTimed(field->shake.rumble - 1, 0);
    } else if (field->shake.rumble > 0)
        GameRumble_PlayTimed(field->shake.rumble - 1, 0);
}

}
