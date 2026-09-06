#include <game/scene_script.h>

extern u8 data_ov005_0206a180[];
extern const void *data_ov007_0208dbbc[];

extern void MI_CpuFill8(void *destination, u32 value, u32 size);
extern void func_ov005_02067468(
    void *archive_manager, int archive_slot, const void *archive_name,
    int load_immediately);
extern void *func_ov005_020670b4(
    void *archive_manager, int archive_slot, int member_id,
    void *size_output, int allocate_from_end);
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
    *(void **)(manager + 0x57DC) = func_ov005_020670b4(
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
