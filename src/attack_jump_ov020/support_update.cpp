/* Updates the support input window and follows its return animation. */
#include "attack_internal.h"
extern "C" {
extern u16 data_ov002_020be704[];

void Overlay20Support_Update(Overlay20AttackState *state, Overlay20AttackState *main)
{
    BattleSceneObject *object = state->primary_object;
    if (!object)
        return;
    Overlay20AttackConfig *config = &data_ov020_020c4fd0[main->flags.bits.formation_variant];
    u16 pressed = *(u16 *)(gBattleContext + 0x104) & 0xC03;
    switch (state->flags.bits.state) {
    case 0:
        break;
    case 1: {
        int upper = config->support_window_before;
        int lower = -config->support_window_after;
        if (lower <= main->motion_duration && main->motion_duration <= upper && !state->flags.bits.mode)
            *(u16 *)(gBattleContext + 0xCB80) |= 2;
        if (pressed && !state->flags.bits.mode) {
            if (pressed == data_ov002_020be704[state->flags.bits.formation_variant] &&
                lower <= main->motion_duration && main->motion_duration <= upper) {
                state->flags.bits.mode = 1;
                Overlay20Attack_SetMode(main, 3);
            } else if (pressed != data_ov002_020be704[main->flags.bits.formation_variant]) {
                state->flags.bits.mode = 2;
                Overlay20Attack_SetMode(main, 2);
            }
            state->flags.bits.state = 0;
        }
        if (lower > main->motion_duration)
            state->flags.bits.state = 0;
        break;
    }
    case 2:
        if (object->z < object->motion_origin_z) {
            func_ov020_020c2efc(state, 0x4302);
            state->flags.bits.state = 3;
        }
        /* Fall through: the arc can finish in the same update. */
    case 3:
        if (!BattleSceneObject_IsAnimationChannelActive(object, 3)) {
            Overlay20Attack_SpawnLandingEffect(state, 16, 17);
            func_ov020_020c2efc(state, 0x4304);
            state->flags.bits.state = 4;
        }
        break;
    case 4:
        if ((u8)BattleSceneObject_GetActiveModel(object)->flag_bits.unknown_09)
            state->flags.bits.state = 0;
        break;
    }
}
}
