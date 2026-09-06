#include <game/scene_script.h>

typedef struct SceneSecondaryArchiveLayout {
    u8 unknown_0000[0x78];
    u8 primary_input_asset;
    u8 secondary_input_asset;
    u8 unknown_007a[0x5766];
    u8 archive_data[0x5000];
    void *temporary_archive;
    u32 temporary_archive_size;
} SceneSecondaryArchiveLayout;

extern u8 data_ov005_0206a180[];
extern const u32 data_02050170[];
extern const u8 gActionItemRecords[];
extern u8 data_ov007_020905f0[];

extern void *func_ov005_020670b4(
    void *archive_manager, int archive_slot, u16 member_id,
    u32 *size_output, int allocate_from_end);
extern void MI_CpuCopy8(
    const void *source, void *destination, u32 size);
extern void func_02029ba4(void *allocation);

enum {
    SCENE_SHARED_MODE_OFFSET = 0x24,
    SCENE_ALTERNATE_INPUT_OFFSET = 0x118,
    SCENE_FORCED_INPUT_MODE_OFFSET = 0x11E,
    ACTION_ITEM_INPUT_FLAGS_OFFSET = 0x10,
    ACTION_ITEM_RECORD_SIZE = 28,
};

void SceneScript_LoadSecondaryArchive(
    SceneSecondaryArchiveLayout *manager, u8 scene_id) {
    u32 record_offset = scene_id * ACTION_ITEM_RECORD_SIZE;
    u16 member_id = *(const u32 *)(
        (const u8 *)data_02050170 + record_offset) & 0xFFF;
    u32 input_kind;

    if (data_ov007_020905f0[SCENE_FORCED_INPUT_MODE_OFFSET] == 4 ||
        (input_kind =
             ((u32)gActionItemRecords[
                  record_offset + ACTION_ITEM_INPUT_FLAGS_OFFSET] << 29) >> 30,
         input_kind == 2)) {
        *(s16 *)(data_ov007_020905f0 + SCENE_SHARED_MODE_OFFSET) = 2;
    } else if (data_ov007_020905f0[SCENE_ALTERNATE_INPUT_OFFSET]) {
        *(s16 *)(data_ov007_020905f0 + SCENE_SHARED_MODE_OFFSET) = 1;
    } else {
        *(s16 *)(data_ov007_020905f0 + SCENE_SHARED_MODE_OFFSET) = 0;
        if (input_kind == 1) {
            *(s16 *)(data_ov007_020905f0 + SCENE_SHARED_MODE_OFFSET) = 2;
        }
    }

    if (*(s16 *)(data_ov007_020905f0 + SCENE_SHARED_MODE_OFFSET) == 0) {
        manager->primary_input_asset = 0;
        manager->secondary_input_asset = 1;
    } else if (
        *(s16 *)(data_ov007_020905f0 + SCENE_SHARED_MODE_OFFSET) == 1) {
        manager->primary_input_asset = 2;
        manager->secondary_input_asset = 3;
    } else if (
        *(s16 *)(data_ov007_020905f0 + SCENE_SHARED_MODE_OFFSET) == 2) {
        manager->primary_input_asset = 4;
        manager->secondary_input_asset = 5;
    }

    manager->temporary_archive = func_ov005_020670b4(
        *(void **)(data_ov005_0206a180 + 0x2C), 4, member_id,
        &manager->temporary_archive_size, 0);
    MI_CpuCopy8(
        manager->temporary_archive, manager->archive_data,
        manager->temporary_archive_size);
    if (manager->temporary_archive != 0) {
        func_02029ba4(manager->temporary_archive);
        manager->temporary_archive = 0;
    }
}
