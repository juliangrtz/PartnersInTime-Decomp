#include <game/battle_object_link.h>
extern void func_0202cbd4(void *, int, u32);
extern int func_ov002_020b82d0(BattleSceneObject *, int);
extern int func_ov002_020b99bc(BattleSceneObject *, int);
BattleObjectLinkDraw BattleObjectLink_InitializeTrail(BattleSceneObject *object,
                                                      BattleSceneObject *other, int spacing,
                                                      int length, int style) {
    BattleObjectLinkOverride *owner =
        (BattleObjectLinkOverride *)BattleSceneObject_ReserveRenderOverride(object, 508);
    BattleObjectLinkState *state = owner->state;
    int count, i, segments;
    int dx, dy, dz;
    func_0202cbd4(state, 0, 508);
    length <<= 8;
    state->target = other->actor_id;
    state->style = style;
    state->gravity_q8 = 48;
    state->unknown_0a = 64;
    state->flags.follow_source = 1;
    state->flags.follow_target = 0;
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
    segments = count - 1;
    dx = (state->end_x - state->start_x) / segments;
    dy = (state->end_y - state->start_y) / segments;
    dz = (state->end_z - state->start_z) / segments;
    for (i = 1; i < count; ++i) {
        state->segments[i].x = (s16)dx;
        state->segments[i].y = (s16)dy;
        state->segments[i].z = (s16)dz;
    }
    owner->draw = func_ov002_020b82d0;
    return func_ov002_020b82d0;
}

void BattleObjectLink_SetFollowing(BattleSceneObject *object, int first, int second) {
    BattleObjectLinkState *s = OBJECT_LINK_STATE(object);
    s->flags.follow_source = first != 0;
    s->flags.follow_target = second != 0;
}

BattleObjectLinkState *BattleObjectLink_SetSourceOffset(BattleSceneObject *object, int x, int y, int z) {
    BattleObjectLinkState *s = OBJECT_LINK_STATE(object);
    s->source_offset_x = x;
    s->source_offset_y = y;
    s->source_offset_z = z;
    return s;
}

void BattleObjectLink_SetSourceOffsetX(BattleSceneObject *object, int value) {
    OBJECT_LINK_STATE(object)->source_offset_x = value;
}

void BattleObjectLink_SetSourceOffsetY(BattleSceneObject *object, int value) {
    OBJECT_LINK_STATE(object)->source_offset_y = value;
}

void BattleObjectLink_SetSourceOffsetZ(BattleSceneObject *object, int value) {
    OBJECT_LINK_STATE(object)->source_offset_z = value;
}

void BattleObjectLink_SetTargetOffset(BattleSceneObject *object, int x, int y, int z) {
    BattleObjectLinkState *s = OBJECT_LINK_STATE(object);
    s->target_offset_x = x;
    s->target_offset_y = y;
    s->target_offset_z = z;
}

void BattleObjectLink_SetGravity(BattleSceneObject *object, int value) {
    OBJECT_LINK_STATE(object)->gravity_q8 = value;
}

void BattleObjectLink_SetParameter0A(BattleSceneObject *object, int value) {
    BattleObjectLinkState *s = OBJECT_LINK_STATE(object);
    s->unknown_0a = value;
    ((BattleObjectLinkTrailView *)s)->unknown_1f8 = s->unknown_0a;
}

void BattleObjectLink_SetGroundParameter(BattleSceneObject *object, int value) {
    ((BattleObjectLinkTrailView *)OBJECT_LINK_STATE(object))->unknown_1f8 = value;
}

void BattleObjectLink_SetParameter0C(BattleSceneObject *object, int value) {
    OBJECT_LINK_STATE(object)->unknown_0c = value;
}

void BattleObjectLink_SetParameter0E(BattleSceneObject *object, int value) {
    OBJECT_LINK_STATE(object)->unknown_0e = value;
}
