#include <game/title_panel_motion.h>
#include <game/random.h>
extern const s8 data_ov006_0207b0b8[];
extern const s16 data_ov006_0207b0cc[];
extern const u16 data_ov006_0207b0bc[], data_ov006_0207b0c4[];
extern const s32 data_ov006_0207b0e0[];
void func_ov006_02073e6c(void *, int, int);

void TitleMovingSprite_ResetOrbit(TitleMovingSprite *work)
{
    unsigned index;
    int animation;
    work->angle = data_ov006_0207b0e0[work->flags.side];
    work->velocity = data_ov006_0207b0c4[work->flags.variant] +
                     Random_NextModulo(data_ov006_0207b0bc[work->flags.variant]);
    work->velocity *= data_ov006_0207b0b8[work->flags.side];
    work->x = data_ov006_0207b0cc[work->flags.side] << 12;
    work->y = 0x100000;
    index = work->flags.side + 2 * work->flags.variant;
    if (!(index & 1)) {
        animation = index + 4 * Random_NextModulo(3);
    } else {
        TitleMovingSprite *peer = &((TitleMovingSprite *)work->parent)[index - 1];
        BattleModel *model = ((TitleMovingSpriteChild *)peer->child)->sub_model;
        animation = model->vtable->get_animation_id(model) + 1;
    }
    func_ov006_02073e6c(work->child, animation, 1);
}
