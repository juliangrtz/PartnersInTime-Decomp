extern "C" {

/*
 * Scene property selectors (overlay 7, 0x020883A0-0x02088468).
 *
 * The get and set selectors the scene scripts read and write scene properties
 * through, plus the animation-layer clear.
 */

#include <game/scene_vm_state.h>
#include <nitro.h>

/* The scene block; only a few of its fields are typed here. */
extern u8 data_ov007_020905f0[];

void func_ov007_02088454(u32 selector, u16 value)
{
    if (selector == 5) *(u16 *)&data_ov007_020905f0[0x24] = value;
}

int func_ov007_020883d0(u32 selector)
{
    switch (selector) {
    case 3:
    case 4:
        return SceneMenu_GetSelectedItem();
    case 5:
        return *(s16 *)&data_ov007_020905f0[0x24];
    case 9:
        return 0x38;
    }
    return 0;
}

}

#include <game/battle_scene.h>

extern "C" {

extern BattleModel *SceneObject_GetModelById(u32 object_id);

/* Clears one animation layer of the object's model. */
int func_ov007_020883a0(u32 object_id, int layer)
{
    return SceneObject_GetModelById(object_id)->configure_animation_layer(
        (s8)layer, -1, 1);
}

}
