#include <game/field_party.h>
#include <game/field_auxiliary.h>
#include <game/field_blink.h>

extern "C" {
void FieldEntity3D_UpdateScreenPosition(FieldRuntimeEntity *, s16, s16);
void func_ov000_020a6d68(FieldEntity *, const void *, int, int, int, int, int);
void GameAudio_PlayEffectDelayed(int, int, int);

void FieldPartyEntity_UpdateScreenPositions(FieldPartyEntity *member, s16 camera_x, s16 camera_y)
{
    FieldEntity3D_UpdateScreenPosition(&member->entity, camera_x, camera_y);
    for (int i = 0; i < 6; i++)
        if (member->auxiliaries[i])
            member->auxiliaries[i]->entity.base.update_screen_position(camera_x, camera_y);
}

void FieldPartyEntity_UpdateAuxiliaryPriorities(FieldPartyEntity *member)
{
    for (int i = 0; i < 6; i++)
        if (member->auxiliaries[i])
            FieldAuxiliary_UpdateRenderPriority(member->auxiliaries[i]);
}

void FieldPartyEntity_CopySortKeysToAnchors(FieldPartyEntity *member)
{
    FieldEntity_CopySortKeysToAnchors(&member->entity);
    for (int i = 0; i < 6; i++)
        if (member->auxiliaries[i])
            member->auxiliaries[i]->entity.base.unknown_70();
}

void FieldPartyEntity_ShowBlinkRenderersWithSound(FieldEntity *entity)
{
    FieldBlink_ShowRenderers(entity);
    GameAudio_PlayEffectDelayed(226, 0, -1);
}

void FieldPartyEntity_HideBlinkRenderers(FieldEntity *entity)
{
    FieldBlink_HideRenderers(entity);
}

void FieldPartyEntity_BindStateResource18(FieldPartyEntity *member)
{
    func_ov000_020a6d68(&member->entity.base, member->state_record->resources.resource_18, 0, 0, -1, 0, 0);
}

void FieldPartyEntity_BindStateResource1C(FieldPartyEntity *member)
{
    func_ov000_020a6d68(&member->entity.base, member->state_record->resources.resource_1c, 0, 0, -1,
                        member->state_record->resources.flags.update_bounds != 0, 0);
    member->state_record->resources.flags.update_bounds = 0;
}
}
