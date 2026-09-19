#include "scene_object_internal.h"

extern "C" {

void SceneModel_Initialize(BattleModel *model, int engine, int slot)
{
    void *resource = func_ov005_020698dc(slot);
    func_ov005_02068908(model, engine, resource, 0, -1);
    model->flag_bits.unknown_00_01 = 1;
}

void SceneModel_Stop(BattleModel *model)
{
    model->unknown_1c();
    model->flag_bits.animation_active = 0;
}

BattleModel *SceneObject_GetModelById(u32 id)
{
    return SceneObject_GetActiveModel((SceneObject *)SceneObject_GetById(id));
}

BattleModel *SceneObject_GetActiveModel(SceneObject *object)
{
    if (object->flags.bits.use_secondary_model)
        return SECONDARY(object);
    return PRIMARY(object);
}

void SceneObject_SetFacingLeft(u32 id, int enabled)
{
    SceneObject_GetModelById(id)->flag_bits.facing_left = enabled;
}

void SceneObject_SetFlipY(u32 id, int enabled)
{
    SceneObject_GetModelById(id)->flag_bits.flip_y = enabled;
}

void SceneObject_SetAnimationLimit(SceneObject *object, int value, int flag)
{
    object->property_0d6 = value;
    object->flags.bits.hide_after_animation = (u16)flag;
}

int SceneObject_UpdateAnimationLimit(SceneObject *object)
{
    BattleModel *model = SceneObject_GetActiveModel(object);
    if (model->flag_bits.panel_animation_trigger) {
        if (object->property_0d6 > 0) {
            --object->property_0d6;
            if (!object->property_0d6) {
                object->property_0d6 = -1;
                if (object->flags.bits.hide_after_animation) {
                    SceneObject_SetAnimation(object, -1, -1);
                    return 0;
                }
                model->flag_bits.unknown_09 = 1;
            }
        }
    }
    return 1;
}

SceneScriptState *SceneScript_GetObjectState(u32 id)
{
    /* Preserve the separate native object-range checks, including the
       unchanged index for IDs at or above 68. */
    if (id < 40)
        id -= 28;
    else if (id < 56)
        id -= 28;
    else if (id < 60)
        id -= 28;
    else if (id < 68)
        id -= 28;
    return (SceneScriptState *)(data_ov007_020a6b90 + 14812 + 192 * id);
}

int SceneResource_LoadSlot(u32 slot, u32 resource)
{
    int archive;
    int compressed;
    void *asset;
    if (slot < 72) {
        if (resource == (u32)META(slot))
            return 0;
        archive = 0;
        compressed = 0;
        META(slot) = (void *)resource;
        /* The high byte chooses one of the six common archives; the lower
           bits identify its member. */
        switch (resource & 0xff000000) {
        case 0xc0000000:
            break;
        case 0xc1000000:
            archive = 1;
            break;
        case 0xc2000000:
            archive = 2;
            break;
        case 0:
            archive = 3;
            compressed = 1;
            break;
        case 0x01000000:
            archive = 4;
            compressed = 1;
            break;
        case 0x02000000:
            archive = 5;
            compressed = 1;
            break;
        }
        if (compressed)
            asset = func_ov005_02066a7c(ARCHIVE, archive + 6, resource & 0x00ffffff,
                                        *(void **)(data_ov007_020a6b90 + 96 + 4 * archive), 0, 0);
        else
            asset = func_ov005_02066920(ARCHIVE, archive + 6, resource & 0x00ffffff,
                                        *(void **)(data_ov007_020a6b90 + 96 + 4 * archive), 0, 0);
        func_ov005_020698c8(slot, asset);
    }
    return 0;
}

int SceneResource_IsReady(int slot)
{
    void *asset = func_ov005_020698dc(slot);
    return func_ov005_02067248(ARCHIVE, *(void **)((u8 *)asset + 16)) == 0;
}

void SceneObject_SetResourceId(u32 id, u32 resource)
{
    if (resource < 72) {
        SceneObject *object = (SceneObject *)SceneObject_GetById(id);
        object->resource_id = resource;
        object->animation_id = 0;
    }
}
}
