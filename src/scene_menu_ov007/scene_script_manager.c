/*
 * Scene script archives (overlay 7, 0x0208462C-0x020848F8).
 *
 * Loads the primary and secondary script archives and starts the scripts in
 * them. The loader and the runtime initializer see the same allocation through
 * different views.
 */

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
    void *archive_manager, int archive_slot, int member_id,
    void *size_output, int allocate_from_end);
extern void MI_CpuCopy8(
    const void *source, void *destination, u32 size);
extern void GameHeap_DeleteArray(void *allocation);

enum {
    SCENE_SHARED_MODE_OFFSET = 0x24,
    SCENE_ALTERNATE_INPUT_OFFSET = 0x118,
    SCENE_FORCED_INPUT_MODE_OFFSET = 0x11E,
    ACTION_ITEM_RECORD_SIZE = sizeof(SceneActionItemInput)
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
        *(void **)(data_ov005_0206a180 + 0x2C), 4, (u16)member_id,
        &manager->temporary_archive_size, 0);
    MI_CpuCopy8(
        manager->temporary_archive, manager->archive_data,
        manager->temporary_archive_size);
    if (manager->temporary_archive != 0) {
        GameHeap_DeleteArray(manager->temporary_archive);
        manager->temporary_archive = 0;
    }
}

extern const void *data_ov007_0208dbbc[];

extern void MI_CpuFill8(void *destination, u32 value, u32 size);
extern void func_ov005_02067468(
    void *archive_manager, int archive_slot, const void *archive_name,
    int load_immediately);
extern int func_ov005_02067424(void *archive_manager, int archive_slot);

typedef struct SceneGlobalScriptSlot {
    const u16 *script;
    u8 unknown_004[0xAC];
    u16 owner;
    u8 unknown_0b2[0x06];
} SceneGlobalScriptSlot;

typedef struct SceneScriptManagerLayout {
    u8 unknown_0000[0x37EC];
    u8 shared_variables[0x80];
    SceneGlobalScriptSlot primary_script;
    SceneGlobalScriptSlot secondary_script;
    u8 unknown_39dc[0x1E00];
    void *primary_archive;
    u16 secondary_archive[1];
} SceneScriptManagerLayout;

/* Metrowerks emits C functions in reverse source order. */
int SceneScript_StartSecondary(u8 *manager) {
    SceneScriptManagerLayout *scene = (SceneScriptManagerLayout *)manager;

    MI_CpuFill8(&scene->secondary_script, 0, 0xB8);
    MI_CpuFill8(scene->shared_variables, 0, 0x80);
    /* script = archive + (archive_size_halfwords >> 1), owner = secondary */
    asm {
        add r0, manager, #0x5700
        ldrh r2, [r0, #0xe0]
        ldr r1, =0x57e0
        ldr r0, =0x3924
        add r3, manager, r1
        mov r1, r2, lsr #1
        add r2, r3, r1, lsl #1
        add r1, manager, #0x3000
        str r2, [r1, #0x924]
        add r1, manager, r0
        mov r0, #4
        strh r0, [r1, #0xb0]
    }
}

int SceneScript_LoadPrimaryArchive(
    u8 *manager, int archive_kind, int member_id) {
    func_ov005_02067468(
        *(void **)(data_ov005_0206a180 + 0x2C), 0,
        data_ov007_0208dbbc[archive_kind + 1], 1);
    *(void **)(manager + 0x57DC) = Overlay5Archive_ReadEntry(
        *(void **)(data_ov005_0206a180 + 0x2C), 0, member_id, 0, 0);
    return func_ov005_02067424(
        *(void **)(data_ov005_0206a180 + 0x2C), 0);
}

int SceneScript_StartPrimary(u8 *manager) {
    SceneScriptManagerLayout *scene = (SceneScriptManagerLayout *)manager;

    MI_CpuFill8(&scene->primary_script, 0, 0xB8);
    /* script = archive + (archive_size_halfwords >> 1), owner = primary */
    asm {
        add r0, manager, #0x5000
        ldr r3, [r0, #0x7dc]
        ldr r0, =0x386c
        ldrh r2, [r3]
        add r1, manager, #0x3000
        add r12, manager, r0
        mov r0, r2, lsr #1
        add r0, r3, r0, lsl #1
        str r0, [r1, #0x86c]
        mov r0, #1
        strh r0, [r12, #0xb0]
    }
}

int SceneScript_IsPrimaryRunning(const u8 *manager) {
    return *(const void *const *)(manager + 0x386C) != 0;
}
