#ifndef PIT_TITLE_ANIMATION_INTERNAL_H
#define PIT_TITLE_ANIMATION_INTERNAL_H
#include <game/title_animation.h>
#include <game/palette_animation.h>
extern "C" {
#include <game/graphics_resource.h>
#include <game/archive_io.h>
}
struct TitleAnimationResourceEntry {
    u16 image_entry, data_index;
    u8 unknown_04[3];
    u8 localized : 1;
    u8 unknown_01_07 : 7;
};
struct TitleAnimationResourceData {
    u16 entry, auxiliary_entry;
    u8 unknown_04[4];
};
struct TitleAnimationResource {
    u32 id;
    GameGraphicsResource *graphics;
    void *unknown_08, *unknown_0c, *unknown_10;
    u16 *screen_group_offsets, *screen_offsets;
    u16 *render_group_offsets, *render_offsets;
    TitleAnimationResourceEntry *entry;
    TitleAnimationResourceData *data;
    u8 owns_screen_group_offsets : 1;
    u8 owns_screen_offsets : 1;
    u8 owns_render_group_offsets : 1;
    u8 owns_render_offsets : 1;
    u8 unknown_04_07 : 4;
    u8 unknown_2d[3];
};
typedef char TitleAnimationResourceEntry_Size[sizeof(TitleAnimationResourceEntry) == 8 ? 1 : -1];
typedef char TitleAnimationResourceData_Size[sizeof(TitleAnimationResourceData) == 8 ? 1 : -1];
typedef char TitleAnimationResource_Size[sizeof(TitleAnimationResource) == 48 ? 1 : -1];
/* Prefix of the 75304-byte animation controller, through its cleared fields. */
struct TitleAnimationController {
    GameTask base;
    u8 unknown_18[16];
    GameIrqTask *irq;
    ArchiveCompressedIO *archive;
    GameTask *frame_task;
    const void *current_descriptor;
    u32 *offsets[5];
    u32 *tables[5];
    u8 *entries[5];
    u8 *entry_data[5];
    TitleAnimationResource resources[16];
    GamePaletteEffectController *palette_effects[2];
    GamePaletteEffectEntry palette_entries[2][2];
    u8 unknown_3e0[48];
    void *localized_resource, *shared_resource;
    u8 language, exit_state, exit_kind;
    u8 previous_first_delay, previous_repeat_delay;
    u8 unknown_41d[7];
};
struct TitleAnimationArchive {
    const void *descriptor;
    u32 resource_prefix;
    u8 table_format, unknown_09[7];
};
typedef char TitleAnimationController_PrefixSize[sizeof(TitleAnimationController) == 1060 ? 1 : -1];
typedef char TitleAnimationArchive_Size[sizeof(TitleAnimationArchive) == 16 ? 1 : -1];

#endif
