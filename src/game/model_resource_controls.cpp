extern "C" {
#include <game/graphics_resource.h>
}
#include <game/model_resource.h>
#include <game/battle_scene.h>
#include <game/sprite_output.h>
#include <game/palette_animation.h>
extern "C" {
extern BattleModelVTable data_02050a24;
void func_0200a45c(BattleModel *, const void *);
void func_02009e38(int, const u16 *, const GameGraphicsResource *);
void func_02009ffc(int, const u16 *, const GameGraphicsResource *, int);
u32 func_02009498(const GameGraphicsResource *);
void func_02009644(const void *, int, u32, int);
int func_02009224(BattleModel *, int);
extern const u8 data_020509c4[88];
extern const u16 *data_0205a8ac;
extern void func_02009148(BattleModel *);
extern void func_0202cbd4(void *, int, u32);
extern void func_0202cc58(const void *, void *, u32);
extern void func_0202cd2c(const void *, void *, u32);
int BattleModel_GetTextureConversionSize(int, int, const GameGraphicsResource *);

void BattleModel_RestoreSourcePalette(BattleModel *model)
{
    model->palette->flags.raw &= ~0x10;
    model->set_palette_dirty(1);
}

void BattleModel_CopyAnimationLayers(BattleModel *source, BattleModel *destination)
{
    for (int layer = 0; layer < 8; ++layer) {
        destination->controller_work.layer_components[layer] =
            source->controller_work.layer_components[layer];
        destination->controller_work.layer_steps_q8[layer] = source->controller_work.layer_steps_q8[layer];
        destination->controller_work.layer_states[layer] = source->controller_work.layer_states[layer];
        destination->controller_work.layer_modes[layer] = source->controller_work.layer_modes[layer];
        destination->controller_work.layer_animation_ids[layer] =
            source->controller_work.layer_animation_ids[layer];
        if (source->controller_work.layer_tracks[layer]) {
            destination->controller_work.layer_tracks[layer] =
                GameAnimationTable_GetTrack((const s16 *)destination->animation_controller,
                                            source->controller_work.layer_animation_ids[layer]);
        } else {
            destination->controller_work.layer_tracks[layer] = 0;
        }
    }
}

void BattleModel_SetPaletteMask(BattleModel *model, GamePaletteEffectController *table, int index,
                                int enabled)
{
    GamePaletteEffectEntry *entry = GamePaletteEffects_GetEntry(table, index);
    GameSpritePalette *palette = model->palette;
    u32 mask;
    if (palette->bank > 15)
        mask = ((1 << palette->count) - 1) << (palette->first + 16);
    else
        mask = 1 << palette->bank;
    if (enabled)
        entry->palette_mask |= mask;
    else
        entry->palette_mask &= ~mask;
}

void BattleModel_SetPaletteBuffered(BattleModel *model, int enabled)
{
    if (enabled)
        model->palette->flags.raw |= 0x10;
    else
        model->palette->flags.raw &= ~0x10;
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

void BattleModel_InitDescriptor(void *descriptor, int unused_mode)
{
    if ((const void *)data_020509c4 < descriptor)
        func_0202cd2c(data_020509c4, descriptor, 88);
    else
        func_0202cc58(data_020509c4, descriptor, 88);
}

BattleModel *BattleModel_InitResourceState(BattleModel *model)
{
    func_02009148(model);
    *(BattleModelVTable **)model = &data_02050a24;
    func_0202cbd4(&model->texture, 0, model->unk_080 - (u8 *)&model->texture);
    return model;
}

int BattleModel_GetScreenTextureConversionSize(int screen, int alternate,
                                               const GameGraphicsResource *resource)
{
    return BattleModel_GetTextureConversionSize((u8)(GameSprite_ObjBoundaryShift(screen) + 5), alternate,
                                                resource);
}

int BattleModel_GetTextureConversionSize(int boundary, int alternate, const GameGraphicsResource *resource)
{
    if (!alternate) {
        if (boundary == resource->flags.bits.normal_boundary)
            return 0;
    } else if (boundary == resource->flags.bits.alternate_boundary) {
        return 0;
    }
    if (BattleModel_FindTextureOffsets(boundary, alternate, resource))
        return 0;
    else
        return 2 * resource->object_count;
}

const u16 *BattleModel_FindTextureOffsets(int boundary, int alternate, const GameGraphicsResource *resource)
{
    u32 layouts = resource->texture_offset_layouts;
    for (int i = 0; i < 4; i++) {
        if (layouts & 7) {
            if ((layouts & 7) == boundary && (layouts & 8) >> 3 == alternate)
                return (const u16 *)GameGraphics_GetSection(resource, i + 6);
        } else
            return 0;
        layouts >>= 4;
    }
    return 0;
}

int BattleModel_InitializeSpriteResources(BattleModel *model, const ModelResourceDescriptor *descriptor)
{
    model->screen = descriptor->resource_flags.screen;
    /* The original reads DISPCNT once before sampling its OBJ boundary bits. */
    *data_0204ff88[model->screen];
    u16 boundary = GameSprite_ObjBoundaryShift(model->screen);
    func_0200a45c(model, descriptor->animation);
    model->resource_pixels = descriptor->graphics;
    const void *source = &descriptor->resource_animation;
    void *destination = &model->animation_id;
    if (source < destination)
        func_0202cd2c(source, destination, model->unk_080 - (u8 *)destination);
    else
        func_0202cc58(source, destination, model->unk_080 - (u8 *)destination);
    model->set_animation((u8)(s16)descriptor->resource_animation, (s16)descriptor->animation_id);
    int speed = model->effect_scale;
    if (speed <= 0)
        speed = -speed;
    model->anchor_offset += speed;
    model->flags &= ~8;
    model->flag_bits.render_linked = 0;
    u32 tiles = descriptor->texture_tile_count;
    if (!tiles) {
        if (model->resource_flag_bits.alternate_resource == 1)
            tiles = model->resource->alternate_tile_counts;
        else
            tiles = model->resource->normal_tile_counts;
        tiles >>= 10 * boundary;
        tiles &= 1023;
    }
    model->flag_bits.texture_allocation_mode = GameSpriteAllocation_Allocate(
        &model->texture, model->screen, (u8)model->flag_bits.texture_allocation_mode, tiles,
        descriptor->resource_flags.alternate, descriptor->primary_id,
        (u8)model->resource_flag_bits.alternate_resource, descriptor->first_texture_tile);
    if (model->flag_bits.texture_allocation_mode == 3)
        model->texture.state &= ~16;
    else
        model->texture.state |= 16;
    model->palette = descriptor->palette;
    model->palette_index =
        model->resource->flags.bits.color256 ? model->palette->bank : model->palette->first;
    if (BattleModel_GetScreenTextureConversionSize(model->screen,
                                                   (u8)model->resource_flag_bits.alternate_resource,
                                                   (const GameGraphicsResource *)descriptor->animation))
        model->texture_offsets = (const u16 *)descriptor->conversion_buffer;
    else
        model->texture_offsets = 0;
    if (model->texture_offsets && !descriptor->resource_flags.unknown_01) {
        if (model->resource_flag_bits.alternate_resource == 1)
            func_02009e38(model->screen, model->texture_offsets, model->resource);
        else
            func_02009ffc(model->screen, model->texture_offsets, model->resource, -1);
    } else if (!model->texture_offsets) {
        model->texture_offsets =
            BattleModel_FindTextureOffsets(GameSprite_ObjBoundaryShift(model->screen) + 5,
                                           model->resource_flag_bits.alternate_resource, model->resource);
    }
    if (model->resource->flags.bits.color256 == 1) {
        if (model->resource_flag_bits.alternate_resource == 1 ||
            model->resource_flag_bits.alternate_resource == 0 && !model->resource_flag_bits.unknown_24) {
            u32 size = func_02009498(model->resource);
            func_02009644(model->resource_pixels, 0, size, model->palette->first);
        }
    }
    if (model->animation_id >= model->unknown_40())
        model->animation_id = 0;
    int length = func_02009224(model, -1);
    if (model->property_056 > length) {
        length = 0;
        model->property_056 = 0;
    }
    return length;
}
}
