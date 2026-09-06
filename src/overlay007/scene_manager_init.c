#include <game/scene_script.h>

typedef struct SceneVmRuntime {
    u32 unknown_00;
    u32 unknown_04;
    void *command_handler;
    const void *opcode_table;
} SceneVmRuntime;

typedef struct SceneManagerInitLayout {
    u8 unknown_0000[0x7C];
    SceneObject objects[56];
    SceneObject *object_slots[56];
    u8 unknown_333c[0x420];
    SceneVmRuntime vm;
    u8 unknown_376c[0x80];
    u8 shared_variables[0x80];
    u8 primary_script[0xB8];
    u8 secondary_script[0xB8];
    SceneScriptState object_scripts[40];
    void *primary_archive;
    u8 secondary_archive[0x5000];
    void *temporary_archive;
    u8 unknown_a7e4[0x08];
    u8 task_script_state[0x40];
} SceneManagerInitLayout;

extern const u8 data_ov007_0208dca4[];

extern void MI_CpuFill8(void *destination, u32 value, u32 size);
extern void func_0203b76c(u16 value, void *destination, u32 size);
extern int func_ov007_02081730(void *vm, void *state);

void SceneManager_InitializeRuntime(SceneManagerInitLayout *manager) {
    int object_id;
    int script_id;
    u32 *object_metadata;
    SceneObject *object;
    SceneObject **slot;
    volatile u16 clear_value;

    manager->temporary_archive = 0;
    manager->primary_archive = 0;
    MI_CpuFill8(manager->primary_script, 0, 0xB8);
    MI_CpuFill8(manager->secondary_script, 0, 0xB8);
    MI_CpuFill8(manager->shared_variables, 0, 0x80);

    clear_value = 0;
    func_0203b76c(clear_value, manager->objects, sizeof(manager->objects));
    object_metadata = (u32 *)manager;
    object = manager->objects;
    object_id = 0;
    do {
        *(u16 *)((u8 *)object_metadata + 0x158) = object_id;
        slot = &manager->object_slots[object_id++];
        *(u16 *)((u8 *)object_metadata + 0x15A) =
            *(u16 *)((u8 *)object_metadata + 0x158);
        *slot = object;
        object_metadata += sizeof(SceneObject) / sizeof(u32);
        object++;
    } while (object_id < 56);

    MI_CpuFill8(
        manager->object_scripts, 0, sizeof(manager->object_scripts));
    for (script_id = 0; script_id < 40; script_id++) {
        manager->object_scripts[script_id].vm_state.script = 0;
    }

    MI_CpuFill8(
        manager->task_script_state, 0, sizeof(manager->task_script_state));
    manager->vm.unknown_00 = 0;
    manager->vm.unknown_04 = 0;
    manager->vm.opcode_table = data_ov007_0208dca4;
    manager->vm.command_handler = func_ov007_02081730;
}
