#include <game/battle_scene.h>
#include <game/battle_effect.h>
#include <game/battle_context.h>
#include <game/battle_link_effect.h>
typedef struct LinkContextView {
    u8 prefix[53124];
    BattleLinkPoint points[64];
} LinkContextView;
#define LINKS ((LinkContextView *)gBattleContext)
void BattleLinkEffect_Update(BattleLinkEffectTask *task) {
    BattleLinkEffectState *state = &task->state;
    BattleSceneObject *source = BattleSceneObject_GetById(state->source);
    BattleSceneObject *target = state->target ? BattleSceneObject_GetById(state->target) : 0;
    int count = state->count;
    BattleLinkPoint *points = &LINKS->points[64 - count];
    int i, end_mass;
    BattlePosition position;
    for (i = 0; i <= 64 - count; ++i) {
        LINKS->points[i].vx = 0;
        LINKS->points[i].vy = 0;
    }
    BattlePosition_StoreViewRelative(&position, source->x, (s16)(source->y - source->z),
                                     (s16)(source->effect_anchor_z + 16 * (256 - source->y)),
                                     source->flags.bits.use_raw_position,
                                     source->flags.bits.use_alternate_model);
    points->x = (position.x - state->source_x) << 8;
    points->y = (position.y - state->source_y) << 8;
    if (source->flags.bits.use_alternate_model == 1)
        points->y -= (state->screen_y + 192) << 8;
    end_mass = target ? state->end_mass : (u32)state->end_mass >> 2;
    if (!end_mass)
        end_mass = 1;
    BattleLinkEffect_SimulateAndDraw(points, count, state->spacing << 4, end_mass,
                                     position.z + state->depth, state->screen_y, state->color,
                                     state->width);
    if (target) {
        int x = LINKS->points[63].x / 256 - state->target_x;
        int y = LINKS->points[63].y / 256 - state->target_y;
        if (target->flags.bits.use_alternate_model == 1)
            y += state->screen_y + 192;
        BattleSceneObject_AdjustPosition(target, x - target->x, source->y - target->y,
                                         source->y - y - target->z);
    }
}
