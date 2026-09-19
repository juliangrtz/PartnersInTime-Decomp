/*
 * Scene render control (overlay 2, 0x020BB1C4-0x020BB2D4).
 *
 * Reserving the render override slot an effect draws through, and the mesh task
 * state query.
 */

#include <game/battle_scene.h>
#include <game/battle_mesh.h>

/* Twelve-byte slots belong to the resource arena. A slot can be reused
   once its recorded owner no longer points to it. */
typedef struct BattleRenderOverrideSlot {
    BattleSceneRenderOverride base;
    void *state;
    u16 state_size;
    u16 owner_id;
} BattleRenderOverrideSlot;

typedef struct BattleRenderOverrideArena {
    u8 *cursor;
    BattleRenderOverrideSlot slots[1];
} BattleRenderOverrideArena;

extern void func_ov002_020bb2d4(BattleMeshTask *task);

int BattleMesh_GetTaskState(BattleSceneObject *object) {
    BattleMeshTask *task;
    if (object->flags.bits.render_mode != 2)
        return 0;
    task = ((BattleMeshResource *)object->resource->data)->task;
    if (!task)
        return 1;
    if (task->update == func_ov002_020bb2d4)
        return 2;
    if (task->update == BattleMesh_UpdateFadeTask)
        return 3;
    return 4;
}

/* Metrowerks emits these contiguous functions in reverse source order. */
BattleSceneRenderOverride *BattleSceneObject_ReserveRenderOverride(BattleSceneObject *object, u32 size) {
    BattleRenderOverrideArena *arena = (BattleRenderOverrideArena *)object->resource->stream_state;
    BattleRenderOverrideSlot *slot = (BattleRenderOverrideSlot *)object->render_override;
    size = (size + 3) & ~3;

    if (!slot || slot->state_size != size) {
        for (slot = arena->slots; ; ++slot) {
            if (!slot->state) {
                slot->state = arena->cursor;
                slot->state_size = size;
                arena->cursor += size;
                break;
            }
            if (BattleSceneObject_GetById(slot->owner_id)->render_override != &slot->base &&
                slot->state_size == size)
                break;
        }
    }
    slot->owner_id = object->actor_id;
    slot->base.render = 0;
    object->render_override = &slot->base;
    return &slot->base;
}
