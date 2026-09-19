/*
 * Colour cycle setup (overlay 0, 0x020691F8-0x02069284).
 *
 * Initializes the palette colour cycling the field runs.
 */

#include <game/field_system.h>
extern void func_0202cbd4(void *, int, u32);
void FieldColorCycle_Initialize(void *unused, u8 initial, u8 target, u8 mode, u8 duration,
                                u8 option_a, u8 option_b, FieldColorCycle *out) {
    func_0202cbd4(out, 0, 8);
    out->bits.initial = initial;
    out->bits.target = target;
    out->initial_duration = mode;
    out->approach_duration = duration;
    out->target_duration = option_a;
    out->return_duration = option_b;
    out->bits.current = initial;
    out->bytes[3] = mode;
}
