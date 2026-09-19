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

typedef struct SceneActionItemInput {
    u8 unknown_00[0x10];
    u8 flags;
    u8 unknown_11[11];
} SceneActionItemInput;

extern u8 data_ov005_0206a180[];
extern const u32 data_02050170[];
extern const u8 gActionItemRecords[];
extern u8 data_ov007_020905f0[];

extern void *Overlay5Archive_ReadEntry(
    void *archive_manager, int archive_slot, u16 member_id,
    u32 *size_output, int allocate_from_end);
extern void MI_CpuCopy8(
    const void *source, void *destination, u32 size);
extern void GameHeap_DeleteArray(void *allocation);

enum {
    SCENE_SHARED_MODE_OFFSET = 0x24,
    SCENE_ALTERNATE_INPUT_OFFSET = 0x118,
    SCENE_FORCED_INPUT_MODE_OFFSET = 0x11E,
    ACTION_ITEM_RECORD_SIZE = sizeof(SceneActionItemInput),
};

void SceneScript_LoadSecondaryArchive(
    SceneSecondaryArchiveLayout *manager, u8 scene_id) {
    u32 record_offset = scene_id * ACTION_ITEM_RECORD_SIZE;
    u16 member_id = *(const u32 *)(
        (const u8 *)data_02050170 + record_offset) & 0xFFF;
    u32 input_kind;

    if (data_ov007_020905f0[SCENE_FORCED_INPUT_MODE_OFFSET] == 4 ||
        (input_kind =
             ((u32)((const SceneActionItemInput *)(
                  gActionItemRecords + record_offset))->flags << 29) >> 30,
         input_kind == 2)) {
        *(s16 *)(data_ov007_020905f0 + SCENE_SHARED_MODE_OFFSET) = 2;
    } else if (!data_ov007_020905f0[SCENE_ALTERNATE_INPUT_OFFSET]) {
        *(s16 *)(data_ov007_020905f0 + SCENE_SHARED_MODE_OFFSET) = 0;
        if (input_kind == 1) {
            *(s16 *)(data_ov007_020905f0 + SCENE_SHARED_MODE_OFFSET) = 2;
        }
    } else {
        *(s16 *)(data_ov007_020905f0 + SCENE_SHARED_MODE_OFFSET) = 1;
    }

    switch (*(s16 *)(data_ov007_020905f0 + SCENE_SHARED_MODE_OFFSET)) {
    case 0:
        manager->primary_input_asset = 0;
        manager->secondary_input_asset = 1;
        break;
    case 1:
        manager->primary_input_asset = 2;
        manager->secondary_input_asset = 3;
        break;
    case 2:
        manager->primary_input_asset = 4;
        manager->secondary_input_asset = 5;
        break;
    }

    manager->temporary_archive = Overlay5Archive_ReadEntry(
        *(void **)(data_ov005_0206a180 + 0x2C), 4, member_id,
        &manager->temporary_archive_size, 0);
    MI_CpuCopy8(
        manager->temporary_archive, manager->archive_data,
        manager->temporary_archive_size);
    if (manager->temporary_archive != 0) {
        GameHeap_DeleteArray(manager->temporary_archive);
        manager->temporary_archive = 0;
    }
}
