/* Four-phase time-hole color cycle (overlay 0, 0x02069060-0x02069284).
 * Hold the initial value, interpolate toward the target, hold the target, and
 * return. A zero hold duration leaves that phase unchanged. Interpolation
 * durations must be nonzero; division truncates toward zero before the result
 * is stored in the five-bit color field.
 */

#include <game/field_system.h>

extern void func_0202cbd4(void *, int, u32);
void FieldColorCycle_Initialize(void *unused, u8 initial, u8 target, u8 initial_duration, u8 approach_duration,
                                u8 target_duration, u8 return_duration, FieldColorCycle *out) {
    func_0202cbd4(out, 0, 8);
    out->bits.initial = initial;
    out->bits.target = target;
    out->initial_duration = initial_duration;
    out->approach_duration = approach_duration;
    out->target_duration = target_duration;
    out->return_duration = return_duration;
    out->bits.current = initial;
    out->bytes[3] = initial_duration;
}

void FieldColorCycle_Update(void *unused, FieldColorCycle *cycle)
{
    if (cycle->bytes[3]) {
        if (--cycle->bytes[3] == 0) {
            ++cycle->bits.phase;
            switch (cycle->bits.phase) {
            case 0: cycle->bytes[3] = cycle->initial_duration; break;
            case 1: cycle->bytes[3] = cycle->approach_duration; break;
            case 2: cycle->bytes[3] = cycle->target_duration; break;
            case 3: cycle->bytes[3] = cycle->return_duration; break;
            }
        }
    }
    switch (cycle->bits.phase) {
    case 0: cycle->bits.current = cycle->bits.initial; break;
    case 1:
        cycle->bits.current = cycle->bytes[3] * ((int)cycle->bits.initial - (int)cycle->bits.target) /
            cycle->approach_duration + cycle->bits.target;
        break;
    case 2: cycle->bits.current = cycle->bits.target; break;
    case 3:
        cycle->bits.current = cycle->bytes[3] * ((int)cycle->bits.target - (int)cycle->bits.initial) /
            cycle->return_duration + cycle->bits.initial;
        break;
    }
}
