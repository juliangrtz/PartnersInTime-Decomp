/* Three-part battle effects (overlay 2, 0x020B8070-0x020B82D0).
 * Princess Shroob entry 4 enables this renderer through script property 111.
 * Actor 0 detaches a segment; other actor IDs are positioned before binding.
 * Parameter meanings beyond their native bias remain unnamed.
 * Definitions follow descending native addresses for Metrowerks section order.
 */
#include <game/battle_three_part.h>
extern "C" {
#include <game/heap.h>
}
struct BattleThreePartSegment {
    u16 actor_id;
    s16 start[3];
    s16 end[3];
    s16 status;
    u8 unknown_10[134];
    s16 order[3];
};
struct BattleThreePartState {
    BattleThreePartSegment parts[3];
    s16 parameter_1d4, parameter_1d6;
};
struct BattleThreePartOverride {
    int (*render)(BattleSceneObject *, int);
    BattleThreePartState *state;
};
typedef char BattleThreePartSegmentSize[sizeof(BattleThreePartSegment) == 156 ? 1 : -1];
typedef char BattleThreePartStateSize[sizeof(BattleThreePartState) == 472 ? 1 : -1];
extern "C" {
extern char data_ov002_020c0640[];
void func_0202cbd4(void *, int, u32);
void func_ov002_0206f3cc(BattleModel *, BattleObjectDataLoadState *, GameSpritePalette *, int);
int func_ov002_020b7544(BattleSceneObject *, int);
void BattleThreePart_Initialize(BattleSceneObject *object)
{
    /* Resource-owned arena state: three 156-byte segments and two parameters. */
    BattleThreePartOverride *slot = (BattleThreePartOverride *)
        BattleSceneObject_ReserveRenderOverride(object, 472);
    BattleThreePartState *state = slot->state;
    func_0202cbd4(state, 0, 472);
    state->parts[0].start[0] = 0;
    state->parts[0].start[1] = 0;
    state->parts[0].start[2] = 0;
    state->parts[1].start[0] = 0;
    state->parts[1].start[1] = 0;
    state->parts[1].start[2] = 0;
    state->parts[2].start[0] = 0;
    state->parts[2].start[1] = 0;
    state->parts[2].start[2] = 0;
    state->parts[0].end[0] = -40;
    state->parts[0].end[1] = 20;
    state->parts[0].end[2] = 0;
    state->parts[1].end[0] = 0;
    state->parts[1].end[1] = -40;
    state->parts[1].end[2] = 0;
    state->parts[2].end[0] = 40;
    state->parts[2].end[1] = 20;
    state->parts[2].end[2] = 0;
    state->parts[0].order[0] = 3;
    state->parts[0].order[1] = 4;
    state->parts[0].order[2] = 5;
    state->parts[1].order[0] = 6;
    state->parts[1].order[1] = 7;
    state->parts[1].order[2] = 8;
    state->parts[2].order[0] = 0;
    state->parts[2].order[1] = 1;
    state->parts[2].order[2] = 2;
    state->parameter_1d4 = 96;
    state->parameter_1d6 = 224;
    BattleModel *model = object->alternate_model;
    /* Alternate renderers have a 304-byte allocation, unlike primary models. */
    if (!model) {
        model = (BattleModel *)GameHeap_New(304, 0, data_ov002_020c0640, 0);
        if (model)
            model = BattleModelController_InitBase(model);
    }
    BattleModel_StopAlternate(model);
    model->flags &= ~0x100;
    func_ov002_0206f3cc(model, object->resource,
        (GameSpritePalette *)object->unk_0cc, object->resource->resource_id);
    if (!object->resource->flags.bits.copy_flag)
        ((u8 *)&model->property_028)[0] &= ~0x10;
    model->stop();
    object->alternate_model = model;
    slot->render = func_ov002_020b7544;
}

void BattleThreePart_PrepareResource(u16 resource_id)
{
    BattleObjectDataLoadState *resource = BattleObjectData_GetLoadState(resource_id);
    /* Preserve the separate native stores, including unrelated flag bits. */
    resource->flags.bits.copy_flag = 0;
    resource->flags.raw = (resource->flags.raw & ~0x80000000) | 0x80000000;
}

void BattleThreePart_SetParameter1D4(BattleSceneObject *object, int value)
{
    BattleThreePartState *state =
        ((BattleThreePartOverride *)object->render_override)->state;
    state->parameter_1d4 = value;
}

void BattleThreePart_SetParameter1D6(BattleSceneObject *object, int value)
{
    BattleThreePartState *state =
        ((BattleThreePartOverride *)object->render_override)->state;
    /* The VM supplies an offset; the stored halfword includes the native bias. */
    state->parameter_1d6 = (u32)value + 192;
}

void BattleThreePart_BindActor(BattleSceneObject *object, u16 actor_id, int channel)
{
    BattleThreePartState *state =
        ((BattleThreePartOverride *)object->render_override)->state;
    BattleThreePartSegment *part = &state->parts[channel];
    if (actor_id) {
        BattleSceneObject *target = BattleSceneObject_GetById(actor_id);
        BattleSceneObject_AdjustPosition(target,
            object->x + part->end[0] - target->x,
            object->y + part->end[1] - target->y,
            part->end[2] - target->z);
    }
    part->actor_id = actor_id;
}

int BattleThreePart_GetStatus(BattleSceneObject *object, int channel)
{
    BattleThreePartState *state =
        ((BattleThreePartOverride *)object->render_override)->state;
    return state->parts[channel].status;
}
}
