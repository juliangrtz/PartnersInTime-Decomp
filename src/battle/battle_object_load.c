#include <game/battle_context.h>
#include <game/battle_object.h>
#include <game/battle_task_queue.h>
#include <game/save_data.h>

enum BattleObjectResourceConstant {
    BATTLE_OBJECT_RESOURCE_GROUP_MASK = 0xFF000000,
    BATTLE_OBJECT_RESOURCE_GROUP_0 = 0xC0000000,
    BATTLE_OBJECT_RESOURCE_GROUP_1 = 0xC1000000,
    BATTLE_OBJECT_RESOURCE_GROUP_2 = 0xC2000000,
    SAVE_LANGUAGE_OFFSET = 0x515
};

extern void *gBattleObjectTextureComponents[3];

BattleQueuedTask *BattleObjectData_QueueLoad(
    BattleObjectDataLoadState *load_state, s32 resource_id) {
    load_state->resource_id = resource_id;
    load_state->component_08 = 0;
    load_state->flags.bits.processing = 1;
    load_state->flags.bits.load_pending = 0;
    return BattleTaskQueue_Enqueue(
        BattleObjectData_ResolveTextureMetadataTask, load_state);
}

void BattleObjectData_ResolveTextureMetadataTask(BattleQueuedTask *task) {
    BattleSceneResource *resource =
        (BattleSceneResource *)task->argument;
    void *source;
    void *component;
    BattleObjectTextureCatalog *catalog;
    BattleObjectTextureSet *texture_sets;
    BattleObjectTextureSet *texture_set;
    BattleObjectTextureVariant *variants;
    BattleObjectTextureVariant *variant;
    u32 group;
    u32 texture_index;
    u32 texture_set_count;
    u32 fallback_offset;
    u16 tail_texture_id;

    if (resource->flags.bits.load_pending) {
        BattleSceneResource *temporary;

        fallback_offset = 0x37608;
        ((BattleObjectFallbackStorage *)
             (gBattleContext + fallback_offset))->source = resource;
        temporary = &((BattleObjectFallbackStorage *)
            (gBattleContext + fallback_offset))->resource;
        temporary->resource_id = resource->resource_id;
        temporary->component_08 = 0;
        temporary->flags.bits.resource_index =
            resource->flags.bits.resource_index;
        temporary->flags.bits.upload_complete =
            resource->flags.bits.upload_complete;
        temporary->flags.bits.processing = 1;
        temporary->flags.bits.load_pending = 1;
        temporary->flags.bits.allocated = resource->flags.bits.allocated;
        temporary->data = (u8 *)(temporary + 1);
        temporary->allocation_size =
            gBattleContext + fallback_offset +
            sizeof(BattleObjectFallbackStorage) - temporary->data;
        resource = temporary;
        task->argument = temporary;
    }

    group = resource->resource_id & BATTLE_OBJECT_RESOURCE_GROUP_MASK;
    switch (group) {
    case BATTLE_OBJECT_RESOURCE_GROUP_0:
        component = gBattleObjectTextureComponents[0];
        source = ((BattleContext *)gBattleContext)
            ->runtime.object_texture_sources[0];
        catalog = ((BattleContext *)gBattleContext)
            ->runtime.object_texture_catalogs[0];
        break;
    case BATTLE_OBJECT_RESOURCE_GROUP_1:
        component = gBattleObjectTextureComponents[1];
        source = ((BattleContext *)gBattleContext)
            ->runtime.object_texture_sources[1];
        catalog = ((BattleContext *)gBattleContext)
            ->runtime.object_texture_catalogs[1];
        break;
    case BATTLE_OBJECT_RESOURCE_GROUP_2:
        component = gBattleObjectTextureComponents[2];
        source = ((BattleContext *)gBattleContext)
            ->runtime.object_texture_sources[2];
        catalog = ((BattleContext *)gBattleContext)
            ->runtime.object_texture_catalogs[2];
        break;
    default:
        for (;;) {
        }
    }

    texture_sets = catalog->texture_sets;
    texture_index =
        resource->resource_id & ~BATTLE_OBJECT_RESOURCE_GROUP_MASK;
    texture_set = &texture_sets[texture_index];
    texture_set_count = catalog->texture_set_count;
    variants = (BattleObjectTextureVariant *)
        &texture_sets[texture_set_count];
    if (texture_set->flags.bits.localized) {
        texture_index += *(u8 *)(gSaveData + SAVE_LANGUAGE_OFFSET);
    }
    texture_set = &texture_sets[texture_index];
    resource->texture_set = texture_set;

    ((BattleContext *)gBattleContext)->texture_body_id =
        texture_set->body_texture_id;
    tail_texture_id =
        variants[texture_set->variant_index].tail_texture_id;
    variant = &variants[texture_set->variant_index];
    ((BattleContext *)gBattleContext)->texture_tail_id =
        tail_texture_id;
    ((BattleContext *)gBattleContext)->texture_optional_id =
        variant->optional_texture_id;
    resource->texture_variant = variant->resource_variant;

    ((BattleContext *)gBattleContext)
        ->texture_upload_requests[3].component = component;
    ((BattleContext *)gBattleContext)
        ->texture_upload_requests[2].component =
        ((BattleContext *)gBattleContext)
            ->texture_upload_requests[3].component;
    ((BattleContext *)gBattleContext)
        ->texture_upload_requests[1].component =
        ((BattleContext *)gBattleContext)
            ->texture_upload_requests[2].component;
    ((BattleContext *)gBattleContext)
        ->texture_upload_requests[0].component =
        ((BattleContext *)gBattleContext)
            ->texture_upload_requests[1].component;

    ((BattleContext *)gBattleContext)
        ->texture_upload_requests[3].source = source;
    ((BattleContext *)gBattleContext)
        ->texture_upload_requests[2].source =
        ((BattleContext *)gBattleContext)->texture_upload_requests[3].source;
    ((BattleContext *)gBattleContext)
        ->texture_upload_requests[1].source =
        ((BattleContext *)gBattleContext)->texture_upload_requests[2].source;
    ((BattleContext *)gBattleContext)
        ->texture_upload_requests[0].source =
        ((BattleContext *)gBattleContext)->texture_upload_requests[1].source;

    task->callback = BattleObjectData_BeginTextureSetupTask;
}
