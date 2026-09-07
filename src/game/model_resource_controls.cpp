#include <game/battle_scene.h>
#include <game/sprite_output.h>
#include <game/palette_animation.h>
extern "C" {
extern BattleModelVTable data_02050a24;
extern const u8 data_020509c4[88];
extern const u16 *data_0205a8ac;
extern void func_02009148(BattleModel *);
extern void func_0202cbd4(void *, int, u32);
extern void func_0202cc58(const void *, void *, u32);
extern void func_0202cd2c(const void *, void *, u32);
extern const u16 *func_0200c224(int, int, const GameGraphicsResource *);
int BattleModel_GetTextureConversionSize(int, int, const GameGraphicsResource *);

void BattleModel_RestoreSourcePalette(BattleModel *model)
{
    model->palette->flags.raw &= ~0x10;
    model->set_palette_dirty(1);
}

void BattleModel_CopyAnimationLayers(BattleModel *source, BattleModel *destination)
{
    for (int layer = 0; layer < 8; ++layer) {
        destination->controller_work.layer_components[layer] = source->controller_work.layer_components[layer];
        destination->controller_work.layer_steps_q8[layer] = source->controller_work.layer_steps_q8[layer];
        destination->controller_work.layer_states[layer] = source->controller_work.layer_states[layer];
        destination->controller_work.layer_modes[layer] = source->controller_work.layer_modes[layer];
        destination->controller_work.layer_animation_ids[layer] = source->controller_work.layer_animation_ids[layer];
        if (source->controller_work.layer_tracks[layer]) {
            destination->controller_work.layer_tracks[layer] = GameAnimationTable_GetTrack(
                (const s16 *)destination->animation_controller, source->controller_work.layer_animation_ids[layer]);
        } else {
            destination->controller_work.layer_tracks[layer] = 0;
        }
    }
}

void BattleModel_SetPaletteMask(BattleModel *model, GamePaletteEffectController *table, int index, int enabled)
{
    GamePaletteEffectEntry *entry = GamePaletteEffects_GetEntry(table, index);
    GameSpritePalette *palette = model->palette;
    u32 mask;
    if (palette->bank > 15) mask = ((1 << palette->count) - 1) << (palette->first + 16);
    else mask = 1 << palette->bank;
    if (enabled) entry->palette_mask |= mask;
    else entry->palette_mask &= ~mask;
}

void BattleModel_SetPaletteBuffered(BattleModel *model, int enabled)
{
    if (enabled) model->palette->flags.raw |= 0x10;
    else model->palette->flags.raw &= ~0x10;
}

int BattleModel_GetPaletteColorCount(BattleModel *model)
{
    return 16 * model->palette->count;
}

const void *BattleModel_GetPaletteSource(BattleModel *model)
{
    return model->palette->data;
}

void BattleModel_SetPalette(BattleModel *model, const void *source)
{
    GameSpritePalette_MarkDirty(model->palette, source);
}

void *BattleModel_GetPaletteBuffer(BattleModel *model)
{
    return GameSpritePalette_GetBuffer(model->palette);
}

void BattleModel_SetInitialAnimation(BattleModel *model, s16 animation)
{
    model->initial_animation_id = animation;
}

u16 BattleModel_GetRelativeAnimationCount(BattleModel *model)
{
    return model->resource->animation_count - (u16)model->initial_animation_id;
}

u16 BattleModel_GetRelativeAnimationId(BattleModel *model)
{
    return model->animation_id - (u16)model->initial_animation_id;
}

void BattleModel_SetSortKeyOverride(const u16 *table)
{
    data_0205a8ac = table;
}

u16 BattleModel_ReadSortKeyOverride(const u16 *unused, int index)
{
    return data_0205a8ac[index];
}

u16 BattleModel_ReadSortKeyHigh(const u16 *key, int unused)
{
    return key[3];
}

u16 BattleModel_ReadSortKeyLow(const u16 *key, int unused)
{
    return key[2];
}

void BattleModel_InitDescriptor(void *descriptor)
{
    if ((const void *)data_020509c4 < descriptor) func_0202cd2c(data_020509c4, descriptor, 88);
    else func_0202cc58(data_020509c4, descriptor, 88);
}

BattleModel *BattleModel_InitResourceState(BattleModel *model)
{
    func_02009148(model);
    *(BattleModelVTable **)model = &data_02050a24;
    func_0202cbd4(&model->texture, 0, model->unk_080 - (u8 *)&model->texture);
    return model;
}

int BattleModel_GetScreenTextureConversionSize(int screen, int alternate, const GameGraphicsResource *resource)
{
    return BattleModel_GetTextureConversionSize((u8)(GameSprite_ObjBoundaryShift(screen) + 5), alternate, resource);
}

int BattleModel_GetTextureConversionSize(int boundary, int alternate, const GameGraphicsResource *resource)
{
    if (!alternate) {
        if (boundary == resource->flags.bits.normal_boundary) return 0;
    } else if (boundary == resource->flags.bits.alternate_boundary) {
        return 0;
    }
    if (func_0200c224(boundary, alternate, resource)) return 0;
    else return 2 * resource->object_count;
}
}
