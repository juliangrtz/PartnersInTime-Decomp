/* Title model resources and owned conversion tables, 0x0206B9EC-0x0206BD9C. */
#include "title_animation_internal.h"
#include <game/battle_scene.h>

extern "C" {
#include <game/heap.h>
#include <game/model_resource.h>

extern const TitleAnimationArchive data_ov006_0207b038[5];
void func_02009ffc(int, const u16 *, const GameGraphicsResource *, int);

void TitleAnimation_LoadModelResource(TitleAnimationController *work, int slot, u32 id)
{
    TitleAnimationResource *resource = &work->resources[slot];
    /* Keep the archive prefix cached while scanning the descriptor table. */
    u32 prefix = id & 0xff000000;
    int archive = -1;
    for (int i = 0; i < 5; ++i) {
        if (prefix == data_ov006_0207b038[i].resource_prefix) {
            archive = i;
            break;
        }
    }

    /* Model records have a 20-byte stride; other archive entries have eight. */
    if ((id & 0xf0000000) == 0xc0000000) {
        resource->entry = (TitleAnimationResourceEntry *)(
            work->entries[archive] + 20 * (id & 0xffffff));
        if (resource->entry->localized)
            resource->entry = (TitleAnimationResourceEntry *)(
                (u8 *)resource->entry + 20 * work->language);
    } else {
        resource->entry = (TitleAnimationResourceEntry *)(
            work->entries[archive] + 8 * (id & 0xffffff));
        if (resource->entry->localized)
            resource->entry = (TitleAnimationResourceEntry *)(
                (u8 *)resource->entry + 8 * work->language);
    }
    TitleAnimationResourceEntry *entry = resource->entry;
    TitleAnimationResourceData *data = (TitleAnimationResourceData *)(
        work->entry_data[archive] + 8 * entry->data_index);
    resource->data = data;
    resource->id = id;
    resource->graphics = (GameGraphicsResource *)TitleAnimation_ReadArchiveEntry(
        work, archive, entry->image_entry, 1, 0, 1, 0);
    resource->unknown_08 = TitleAnimation_ReadArchiveEntry(
        work, archive, entry->image_entry + 1, 1, 0, 1, 0);
    resource->unknown_0c = TitleAnimation_ReadArchiveEntry(
        work, archive, data->entry, 0, 0, 1, 0);
    if (data->auxiliary_entry != 0xffff)
        resource->unknown_10 = TitleAnimation_ReadArchiveEntry(
            work, archive, data->auxiliary_entry, 0, 0, 1, 0);

    /* Nonzero conversion sizes produce owned arrays, rounded to halfwords. */
    u32 size = BattleModel_GetScreenTextureConversionSize(1, 0, resource->graphics);
    if (size) {
        resource->screen_group_offsets = (u16 *)GameHeap_NewArray(2 * (size >> 1), 1, 0, 0);
        func_02009ffc(1, resource->screen_group_offsets, resource->graphics, -1);
        resource->owns_screen_group_offsets = 1;
    } else {
        resource->screen_group_offsets = (u16 *)BattleModel_FindTextureOffsets(
            GameSprite_ObjBoundaryShift(1) + 5, 0, resource->graphics);
        resource->owns_screen_group_offsets = 0;
    }

    size = BattleModel_GetScreenTextureConversionSize(1, 1, resource->graphics);
    if (size) {
        resource->screen_offsets = (u16 *)GameHeap_NewArray(2 * (size >> 1), 1, 0, 0);
        GameGraphics_BuildScreenTextureOffsets(1, resource->screen_offsets, resource->graphics);
        resource->owns_screen_offsets = 1;
    } else {
        resource->screen_offsets = (u16 *)BattleModel_FindTextureOffsets(
            GameSprite_ObjBoundaryShift(1) + 5, 1, resource->graphics);
        resource->owns_screen_offsets = 0;
    }

    size = BattleRenderModel_GetTextureConversionSize(resource->graphics, 0);
    if (size) {
        resource->render_group_offsets = (u16 *)GameHeap_NewArray(2 * (size >> 1), 1, 0, 0);
        GameGraphics_BuildGroupTextureOffsets(resource->render_group_offsets, resource->graphics, -1);
        resource->owns_render_group_offsets = 1;
    } else {
        resource->render_group_offsets = (u16 *)BattleModel_FindTextureOffsets(3, 0, resource->graphics);
        resource->owns_render_group_offsets = 0;
    }

    size = BattleRenderModel_GetTextureConversionSize(resource->graphics, 1);
    if (size) {
        resource->render_offsets = (u16 *)GameHeap_NewArray(2 * (size >> 1), 1, 0, 0);
        GameGraphics_BuildTextureOffsets(resource->render_offsets, resource->graphics);
        resource->owns_render_offsets = 1;
    } else {
        resource->render_offsets = (u16 *)BattleModel_FindTextureOffsets(3, 1, resource->graphics);
        resource->owns_render_offsets = 0;
    }
}
}
