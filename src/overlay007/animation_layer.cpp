#include <game/battle_scene.h>

extern "C" {

extern BattleModel *func_ov007_020894a8(u32 object_id);

/* Clears one animation layer of the object's model. */
int func_ov007_020883a0(u32 object_id, int layer)
{
    return func_ov007_020894a8(object_id)->configure_animation_layer(
        (s8)layer, -1, 1);
}

}
