#include <game/battle_ai.h>
#include <game/battle_common_assets.h>
#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/battle_object.h>
#include <game/battle_scene.h>
#include <game/save_data.h>
#include <nitro/fx.h>

/*
 * Shared high-numbered battle VM commands. BattleAI_DispatchOpcode delegates
 * opcodes outside its 0x033..0x0E8 extension here. The original function only
 * implements 0x0E9..0x103; unimplemented slots deliberately return without
 * changing the command result.
 */

extern void func_ov002_02069530(int object_data_slot, int catalog_slot);
extern void func_ov002_020691b8(BattleSceneObject *object, int attachment_mode,
                                int catalog_or_value, int element_index);
extern void func_ov002_02068b50(BattleSceneObject *object, int property_id,
                                int element_index, int value, int unused);
extern int func_ov002_02068970(BattleSceneObject *object, int property_id,
                               int element_index);
extern void func_ov002_02068878(BattleObjectDataLoadState *load_state,
                                int control_mode, int value);
extern void func_ov002_020687ec(BattleSceneObject *object, int property_id,
                                int value, int unused);
extern int func_ov002_02068770(BattleSceneObject *object, int property_id,
                               int element_index, int unused);
extern int func_ov002_020661d8(int effect_handle);
extern int func_ov002_02066290(int effect_handle);
extern void func_ov002_0206615c(int effect_handle);
extern int func_ov002_02066314(
    int content_id, int horizontal_flip, int x, int y, int style_id,
    int variant, int auto_vertical, int auto_horizontal, int priority_flag,
    int alignment, int content_width, int content_height, int invert_flag,
    int task_parameter, int resource_parameter, int save_word_40,
    void *save_words);
extern int func_ov002_020665cc(
    int content_id, BattleSceneObject *anchor, int style_id,
    int placement_mode, int auto_vertical, int auto_horizontal,
    int x_offset_or_auto, int content_width, int content_height,
    int invert_flag, int task_parameter, int resource_parameter,
    int save_word_40, void *save_words);
extern int func_ov002_020660fc(int attached_effect_id, int property_id);
extern void func_ov002_02066004(int owner_object_id, int alignment_mode,
                                int reference_object_id, int view_offset,
                                int unused_4, int unused_5);
extern void func_ov002_02065dfc(void);
extern s32 _s32_div_f(s32 numerator, s32 denominator);
extern u8 *data_ov002_020c0660;

enum BattleVmCommonOpcode {
    BATTLE_VM_PREPARE_OBJECT_DATA_ANIMATION_CATALOG = 0x0E9,
    BATTLE_VM_LEGACY_NOOP_0EA = 0x0EA,
    BATTLE_VM_LEGACY_NOOP_0EB = 0x0EB,
    BATTLE_VM_CONFIGURE_OBJECT_ATTACHMENT = 0x0EC,
    BATTLE_VM_LEGACY_NOOP_0ED = 0x0ED,
    BATTLE_VM_SET_ATTACHED_MODEL_PROPERTY = 0x0EE,
    BATTLE_VM_GET_ATTACHED_MODEL_PROPERTY = 0x0EF,
    BATTLE_VM_LEGACY_NOOP_0F0 = 0x0F0,
    BATTLE_VM_LEGACY_NOOP_0F1 = 0x0F1,
    BATTLE_VM_CONTROL_OBJECT_DATA_AUXILIARY_MODEL = 0x0F2,
    BATTLE_VM_SET_ACTIVE_MODEL_AUX_PROPERTY = 0x0F3,
    BATTLE_VM_GET_ACTIVE_MODEL_AUX_PROPERTY = 0x0F4,
    BATTLE_VM_REQUEST_COMMON_ASSETS = 0x0F5,
    BATTLE_VM_WAIT_COMMON_ASSETS = 0x0F6,
    BATTLE_VM_CREATE_BATTLE_UI_ELEMENT = 0x0F7,
    BATTLE_VM_CREATE_OBJECT_ANCHORED_BATTLE_UI_ELEMENT = 0x0F8,
    BATTLE_VM_WAIT_EFFECT_PAIR = 0x0F9,
    BATTLE_VM_WAIT_EFFECT = 0x0FA,
    BATTLE_VM_STOP_EFFECT = 0x0FB,
    BATTLE_VM_LEGACY_NOOP_0FC = 0x0FC,
    BATTLE_VM_MOVE_ATTACHED_OBJECT_AT_SPEED = 0x0FD,
    BATTLE_VM_WAIT_ATTACHED_ANIMATION = 0x0FE,
    BATTLE_VM_GET_ATTACHED_EFFECT_PROPERTY = 0x0FF,
    BATTLE_VM_ALIGN_ATTACHED_OBJECT_VIEW_BASELINE = 0x100,
    BATTLE_VM_START_PRIMARY_SCREEN_EFFECT = 0x101,
    BATTLE_VM_START_SECONDARY_SCREEN_EFFECT = 0x102,
    BATTLE_VM_RESET_SCREEN_EFFECTS = 0x103
};

enum BattleVmCommonLayout {
    BATTLE_VM_ATTACHMENT_OWNER_OFFSET = 0x208,
    BATTLE_VM_ATTACHMENT_TABLE_OFFSET = 0xF00,
    BATTLE_VM_ATTACHMENT_OWNER_STRIDE = 0xCC,
    BATTLE_VM_ATTACHMENT_OFFSET = 0xC4,
    BATTLE_VM_SAVE_WORDS_OFFSET = 0x44
};

typedef struct BattleVmEmbeddedObject {
    u8 unknown_000[8];
    BattleSceneObject scene_object;
} BattleVmEmbeddedObject;

typedef struct BattleVmAttachmentOwner {
    u8 unknown_000[BATTLE_VM_ATTACHMENT_OFFSET];
    BattleVmEmbeddedObject *attachment;
    u8 unknown_0c8[
        BATTLE_VM_ATTACHMENT_OWNER_STRIDE - BATTLE_VM_ATTACHMENT_OFFSET - 4];
} BattleVmAttachmentOwner;

typedef char BattleVmAttachmentOwner_SizeCheck[
    sizeof(BattleVmAttachmentOwner) == BATTLE_VM_ATTACHMENT_OWNER_STRIDE
        ? 1
        : -1];

static inline BattleVmEmbeddedObject *BattleVm_GetAttachment(
    volatile s32 *arguments) {
    return (*(BattleVmAttachmentOwner **)(
        *(u8 **)(data_ov002_020c0660 +
                 BATTLE_VM_ATTACHMENT_OWNER_OFFSET) +
        BATTLE_VM_ATTACHMENT_TABLE_OFFSET))[arguments[0]].attachment;
}

static inline int BattleVm_RetryCommonCommand(
    ScriptVm *vm, ScriptVmState *state, ScriptVmCommand *command) {
    s32 descriptor =
        (u16)vm->command_descriptors[command->opcode];
    int halfword_count =
        ((descriptor & SCRIPT_VM_HAS_RESULT) >> 5) +
        (descriptor & SCRIPT_VM_ARGUMENT_COUNT_MASK) +
        ((descriptor & SCRIPT_VM_HAS_ARGUMENT_MODES) >> 6) + 1;

    state->script -= halfword_count;
    return BATTLE_AI_VM_YIELD;
}

static inline void BattleVm_WriteCommonResult(
    ScriptVm *vm, ScriptVmState *state, ScriptVmCommand *command, int value) {
    VM_WriteVariable(command->result_variable, value, vm, state);
}

int BattleVm_DispatchCommonOpcode(ScriptVm *vm, ScriptVmState *state,
                                  ScriptVmCommand *command) {
    volatile s32 *arguments = command->arguments;
    BattleVmEmbeddedObject *attachment;
    BattleSceneObject *object;
    int value;

    switch (command->opcode) {
    case BATTLE_VM_PREPARE_OBJECT_DATA_ANIMATION_CATALOG:
        func_ov002_02069530(arguments[0], arguments[1]);
        return SCRIPT_VM_CONTINUE;

    case BATTLE_VM_CONFIGURE_OBJECT_ATTACHMENT:
        object = BattleSceneObject_GetById((u16)arguments[0]);
        func_ov002_020691b8(
            object, arguments[1], arguments[2], arguments[3]);
        return SCRIPT_VM_CONTINUE;

    case BATTLE_VM_SET_ATTACHED_MODEL_PROPERTY:
        object = BattleSceneObject_GetById((u16)arguments[0]);
        func_ov002_02068b50(
            object, arguments[1], arguments[2], arguments[3], arguments[4]);
        return SCRIPT_VM_CONTINUE;

    case BATTLE_VM_GET_ATTACHED_MODEL_PROPERTY:
        object = BattleSceneObject_GetById((u16)arguments[0]);
        value = func_ov002_02068970(object, arguments[1], arguments[2]);
        BattleVm_WriteCommonResult(vm, state, command, value);
        return SCRIPT_VM_CONTINUE;

    case BATTLE_VM_LEGACY_NOOP_0F0:
        return SCRIPT_VM_CONTINUE;

    case BATTLE_VM_CONTROL_OBJECT_DATA_AUXILIARY_MODEL:
        func_ov002_02068878(
            BattleObjectData_GetLoadState((u16)arguments[0]),
            arguments[1], arguments[2]);
        return SCRIPT_VM_CONTINUE;

    case BATTLE_VM_SET_ACTIVE_MODEL_AUX_PROPERTY:
        object = BattleSceneObject_GetById((u16)arguments[0]);
        func_ov002_020687ec(
            object, arguments[1], arguments[2], arguments[3]);
        return SCRIPT_VM_CONTINUE;

    case BATTLE_VM_GET_ACTIVE_MODEL_AUX_PROPERTY:
        object = BattleSceneObject_GetById((u16)arguments[0]);
        value = func_ov002_02068770(
            object, arguments[1], arguments[2], arguments[3]);
        BattleVm_WriteCommonResult(vm, state, command, value);
        return SCRIPT_VM_CONTINUE;

    case BATTLE_VM_REQUEST_COMMON_ASSETS:
        BattleCommonAssets_RequestLoad(arguments[0]);
        return SCRIPT_VM_CONTINUE;

    case BATTLE_VM_WAIT_COMMON_ASSETS:
        if (!BattleContext_GetRuntimeState()
                 ->flags.bits.common_asset_load_pending) {
            return SCRIPT_VM_CONTINUE;
        }
        return BattleVm_RetryCommonCommand(vm, state, command);

    case BATTLE_VM_CREATE_BATTLE_UI_ELEMENT:
        value = func_ov002_02066314(
            arguments[1], arguments[0], arguments[2], arguments[3],
            arguments[4], arguments[5], arguments[6], arguments[7],
            arguments[8], arguments[9], arguments[10], arguments[11],
            arguments[12] ^ 1, arguments[13], arguments[14],
            VM_ReadVariable(SCRIPT_VM_VAR_SAVE_WORDS_40, vm, state),
            gSaveData + BATTLE_VM_SAVE_WORDS_OFFSET);
        BattleVm_WriteCommonResult(vm, state, command, value);
        return SCRIPT_VM_CONTINUE;

    case BATTLE_VM_CREATE_OBJECT_ANCHORED_BATTLE_UI_ELEMENT:
        object = BattleSceneObject_GetById((u16)arguments[1]);
        value = func_ov002_020665cc(
            arguments[0], object, arguments[2], arguments[3], arguments[4],
            arguments[5], arguments[6], arguments[7], arguments[8],
            arguments[9] ^ 1, arguments[10], arguments[11],
            VM_ReadVariable(SCRIPT_VM_VAR_SAVE_WORDS_40, vm, state),
            gSaveData + BATTLE_VM_SAVE_WORDS_OFFSET);
        BattleVm_WriteCommonResult(vm, state, command, value);
        return SCRIPT_VM_CONTINUE;

    case BATTLE_VM_WAIT_EFFECT_PAIR:
        if (func_ov002_020661d8(arguments[0]) != 0 &&
            func_ov002_02066290(arguments[0]) != 0) {
            return BattleVm_RetryCommonCommand(vm, state, command);
        }
        return SCRIPT_VM_CONTINUE;

    case BATTLE_VM_WAIT_EFFECT:
        if (func_ov002_020661d8(arguments[0]) != 0) {
            return BattleVm_RetryCommonCommand(vm, state, command);
        }
        return SCRIPT_VM_CONTINUE;

    case BATTLE_VM_STOP_EFFECT:
        func_ov002_0206615c(arguments[0]);
        return SCRIPT_VM_CONTINUE;

    case BATTLE_VM_LEGACY_NOOP_0FC:
        return SCRIPT_VM_CONTINUE;

    case BATTLE_VM_MOVE_ATTACHED_OBJECT_AT_SPEED: {
        int delta_x;
        int delta_y;
        int delta_z;

        attachment = BattleVm_GetAttachment(arguments);
        if (attachment == 0) {
            return SCRIPT_VM_CONTINUE;
        }

        delta_x = arguments[3] - attachment->scene_object.x;
        delta_y = arguments[4] - attachment->scene_object.y;
        delta_z = arguments[5] - attachment->scene_object.z;
        if (arguments[6] != 0) {
            arguments[6] = _s32_div_f(
                FX_Sqrt((delta_x * delta_x + delta_y * delta_y +
                         delta_z * delta_z) << FX32B_INT),
                arguments[6]);
        }
        if (arguments[2] == 1) {
            BattleSceneObject_MoveBy(
                &attachment->scene_object, arguments[1], arguments[3],
                arguments[4], arguments[5], arguments[6]);
        }
        return SCRIPT_VM_CONTINUE;
    }

    case BATTLE_VM_WAIT_ATTACHED_ANIMATION:
        attachment = BattleVm_GetAttachment(arguments);
        if (attachment == 0) {
            return SCRIPT_VM_CONTINUE;
        }
        if (!BattleSceneObject_IsAnimationChannelActive(
                &attachment->scene_object, arguments[1])) {
            return SCRIPT_VM_CONTINUE;
        }
        return BattleVm_RetryCommonCommand(vm, state, command);

    case BATTLE_VM_GET_ATTACHED_EFFECT_PROPERTY:
        value = func_ov002_020660fc(arguments[0], arguments[1]);
        BattleVm_WriteCommonResult(vm, state, command, value);
        return SCRIPT_VM_CONTINUE;

    case BATTLE_VM_ALIGN_ATTACHED_OBJECT_VIEW_BASELINE:
        func_ov002_02066004(
            arguments[0], arguments[1], arguments[2], arguments[3],
            arguments[4], arguments[5]);
        return SCRIPT_VM_CONTINUE;

    case BATTLE_VM_START_PRIMARY_SCREEN_EFFECT:
        BattleScreenEffect_StartPrimary(
            arguments[0], arguments[1], arguments[2]);
        return SCRIPT_VM_CONTINUE;

    case BATTLE_VM_START_SECONDARY_SCREEN_EFFECT:
        BattleScreenEffect_StartSecondary(
            arguments[0], arguments[1], arguments[2]);
        return SCRIPT_VM_CONTINUE;

    case BATTLE_VM_RESET_SCREEN_EFFECTS:
        func_ov002_02065dfc();
        return SCRIPT_VM_CONTINUE;

    case BATTLE_VM_LEGACY_NOOP_0EA:
    case BATTLE_VM_LEGACY_NOOP_0EB:
    case BATTLE_VM_LEGACY_NOOP_0ED:
    case BATTLE_VM_LEGACY_NOOP_0F1:
    default:
        return SCRIPT_VM_CONTINUE;
    }
}
