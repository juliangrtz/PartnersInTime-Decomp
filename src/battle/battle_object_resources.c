#include <game/battle_object.h>

extern u8 *gBattleSystem;

void func_0202cc58(const void *source, void *destination, u32 size);
void func_0202cd2c(const void *source, void *destination, u32 size);
void *func_ov002_020725a4(void (*callback)(void *task), void *argument,
                          int unknown_2, int unknown_3);
void func_ov002_020894e0(void *task);

void BattleObjectData_CopyResource(BattleSceneResource *resource,
                                   BattleObjectDataLoadState *load_state) {
    u32 copy_size = resource->allocation_size;

    if (resource->stream_state != 0) {
        copy_size -= *(u8 **)resource->stream_state -
                     (u8 *)resource->stream_state;
    }

    if (resource->data < load_state->data) {
        func_0202cd2c(resource->data, load_state->data, copy_size);
    } else {
        func_0202cc58(resource->data, load_state->data, copy_size);
    }

    load_state->component_04 = resource->component_04 == 0
        ? 0
        : (void *)((u32)load_state->data + (u32)resource->component_04 -
                   (u32)resource->data);
    load_state->component_08 = resource->component_08 == 0
        ? 0
        : (void *)((u32)load_state->data + (u32)resource->component_08 -
                   (u32)resource->data);
    load_state->component_0c = resource->component_0c == 0
        ? 0
        : (void *)((u32)load_state->data + (u32)resource->component_0c -
                   (u32)resource->data);
    load_state->component_10 = resource->component_10 == 0
        ? 0
        : (void *)((u32)load_state->data + (u32)resource->component_10 -
                   (u32)resource->data);
    load_state->component_14 = resource->component_14 == 0
        ? 0
        : (void *)((u32)load_state->data + (u32)resource->component_14 -
                   (u32)resource->data);
    load_state->unknown_18 = resource->unknown_18;
    load_state->stream_state = 0;
    load_state->unknown_22 = resource->unknown_22;
    load_state->resource_id = resource->resource_id;
    load_state->flags.bits.copy_flag = resource->flags.bits.copy_flag;
    load_state->flags.bits.upload_complete =
        resource->flags.bits.upload_complete;

    if (load_state->flags.bits.resource_index == 0 ||
        load_state->flags.bits.upload_complete) {
        return;
    }

    func_ov002_020725a4(func_ov002_020894e0, load_state, 0, 0);
    *(u16 *)(gBattleSystem + 3574) =
        (*(u16 *)(gBattleSystem + 3574) & ~1) | 1;
}
