#ifndef PIT_GAME_MODEL_RESOURCE_H
#define PIT_GAME_MODEL_RESOURCE_H
#include <game/sprite_output.h>
struct BattleModel;
struct GameGraphicsResource;
/* The 88-byte renderer descriptor used by sprite resource configuration.
 * Its final 44 bytes initialize the renderer presentation record. */
typedef struct ModelResourceDescriptor {
    void *animation;
    u32 primary_id, secondary_id;
    void *graphics, *secondary_data, *conversion_buffer;
    u32 graphics_size;
    u16 secondary_extent;
    struct {
        u8 unknown_00 : 1, unknown_01 : 1, screen : 2, alternate : 1, unknown_05_07 : 3;
    } resource_flags;
    u8 unknown_1f;
    u32 first_texture_tile, texture_tile_count;
    GameSpritePalette *palette;
    u16 resource_animation, animation_id, unknown_30;
    s16 animation_speed;
    s16 animation_offset_x, animation_offset_y;
    u8 unknown_38[0x14];
    s16 scale_x, scale_y, rotation;
    u16 unknown_52;
    struct {
        u32 unknown_00_04 : 5, unknown_05_07 : 3, animation_active : 1, unknown_09 : 1;
        u32 unknown_10 : 1, unknown_11 : 1, behavior_state : 4;
        u32 unknown_16_18 : 3, palette_allocation : 3, resource_set : 1, unknown_23 : 1, unknown_24 : 1,
            unknown_25 : 1, unknown_26_31 : 6;
    } flags;
} ModelResourceDescriptor;
typedef char ModelResourceDescriptor_SizeCheck[sizeof(ModelResourceDescriptor) == 88 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
int BattleModel_InitializeSpriteResources(struct BattleModel *model,
                                          const ModelResourceDescriptor *descriptor);
const u16 *BattleModel_FindTextureOffsets(int boundary, int alternate,
                                          const struct GameGraphicsResource *resource);
#ifdef __cplusplus
}
#endif
#endif
