/*
 * Battle object sprite tiles (overlay 2, 0x020680B0-0x020681E8).
 * The common workspace begins with sixteen allocation records. A resource key
 * combines the archive group with the texture set's body ID, allowing matching
 * sprite allocations to be released when the resource is discarded.
 */
#include <game/battle_object.h>
#include <game/battle_attack_loader.h>
#include <game/sprite_output.h>

u32 func_02009474(const void *resource, int kind, int component);
int func_0200b374(u32 destination, const void *resource, const void *component, int component_index,
                  int end_index);

/* MWCC emits these definitions in reverse order. */
int BattleObjectData_UploadSprites(BattleObjectDataLoadState *resource)
{
    GameSpriteAllocation *allocation = (GameSpriteAllocation *)data_ov002_020c0660;
    int i;
    u32 tiles;
    for (i = 0; i < 16; ++i, ++allocation) {
        if (!allocation->flags.bits.linked)
            break;
    }
    /* Preserve the native fallthrough to the following workspace allocation
       when all sixteen slots are linked; the original has no exhaustion check. */
    tiles = func_02009474(resource->data, 1, 2);
    GameSpriteAllocation_Allocate(
        allocation, 1, 1, tiles, 1,
        resource->texture_set->body_texture_id | (resource->resource_id & 0xFF000000), 1, 0);
    return func_0200b374(allocation->offset + 0x06600000, resource->data, resource->component_14, 2,
                         -1);
}

void BattleObjectData_ReleaseSprites(BattleObjectDataLoadState *resource)
{
    /* Copied resources share their graphics; only the original releases them. */
    if (!resource->flags.bits.copy_flag && resource->texture_set) {
        GameSpriteAllocation *allocation = (GameSpriteAllocation *)data_ov002_020c0660;
        u32 key = resource->texture_set->body_texture_id | (resource->resource_id & 0xFF000000);
        int i;
        for (i = 0; i < 16; ++i, ++allocation) {
            if (allocation->flags.bits.linked && allocation->resource == key)
                GameSpriteAllocation_Unlink(allocation);
        }
    }
}
