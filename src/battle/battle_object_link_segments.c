#include <game/battle_object_link.h>
extern void func_0202cbd4(void *, int, u32);
extern int func_ov002_020b82d0(BattleSceneObject *, int);
extern int func_ov002_020b99bc(BattleSceneObject *, int);
BattleObjectLinkDraw BattleObjectLink_InitializeSegments(BattleSceneObject *object,
                                                         BattleSceneObject *other, int spacing,
                                                         int length, int style, int x, int y,
                                                         int value) {
    BattleObjectLinkOverride *owner =
        (BattleObjectLinkOverride *)BattleSceneObject_ReserveRenderOverride(object, 252);
    BattleObjectLinkState *state = owner->state;
    int count, i, segments;
    int dx, dy, dz;
    func_0202cbd4(state, 0, 252);
    length <<= 8;
    state->target = other->actor_id;
    state->style = style;
    state->gravity_q8 = 48;
    state->unknown_0a = 0;
    state->flags.follow_source = 0;
    state->flags.follow_target = 1;
    state->unknown_0e = 2;
    count = length / spacing + 2;
    state->count = count;
    state->step = length / count;
    state->start_x = object->x << 8;
    state->start_y = object->y << 8;
    state->start_z = object->z << 8;
    state->end_x = other->x << 8;
    state->end_y = other->y << 8;
    state->end_z = other->z << 8;
    state->source_offset_x = x;
    state->source_offset_y = y;
    state->unknown_0c = value;
    segments = count - 1;
    dx = (state->end_x - state->start_x) / segments;
    dy = (state->end_y - state->start_y) / segments;
    dz = (state->end_z - state->start_z) / segments;
    for (i = 1; i < count; ++i) {
        state->segments[i].x = (s16)dx;
        state->segments[i].y = (s16)dy;
        state->segments[i].z = (s16)dz;
    }
    owner->draw = func_ov002_020b99bc;
    return func_ov002_020b99bc;
}
