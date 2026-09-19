/* Update room effects, then run entity movement, completion and render-priority
 * passes. Re-read list links and auxiliary slots after callbacks: they are live
 * state, and a callback can change the next object visited. */
#include <game/field_entity.h>
#include <game/field_entity_motion.h>
#include <game/field_room_transition.h>
#include <game/field_system.h>
extern "C"
{
    void func_ov000_02075450(FieldAreaContext *);
    void func_ov000_02074000(FieldAreaContext *);
    void func_ov000_0206f768(FieldAreaContext *);
    void func_ov000_0207645c(FieldAreaContext *);
    void func_ov000_0206f110(FieldAreaContext *);
    void func_ov000_02071ebc(FieldAreaContext *);
    void func_ov000_020b99b8(FieldPartyEntity *);

    void FieldArea_UpdateEntities(FieldAreaContext *area)
    {
        if (!area->system || area->system->scene_transition.phase == 0 ||
            area->system->scene_transition.phase == 3 || area->system->scene_transition.phase == 5 ||
            area->system->scene_transition.phase == 7 || area->system->scene_transition.phase == 9 ||
            area->system->scene_transition.phase == 11)
        {
            FieldArea_UpdateBrightness(area);
            FieldArea_UpdateAlphaBlend(area);
            func_ov000_02075450(area);
            func_ov000_02074000(area);
            if (area->transition.flags.pending &&
                (!area->transition.flags.fading || !area->unknown_2402.unknown_00))
            {
                FieldArea_PrepareRoomDeparture(area);
                return;
            }
            if (area->party->pending_transition.active && area->flags.screen == 1 &&
                !area->unknown_2402.unknown_00)
                FieldPartyManager_ApplyPendingTransition(area->party);
            func_ov000_0206f768(area);
            func_ov000_0207645c(area);
        }
        FieldArea_UpdateMessageWindowSlide(area);
        func_ov000_0206f110(area);
        for (FieldRuntimeEntity *entity = area->first_entity; entity; entity = entity->base.update_next)
        {
            if (entity->base.property_00a_bits.subtype != 9 && !entity->base.visibility_bits.unknown_08 &&
                (entity->base_state_flag_bits.retain_offscreen_contact ||
                 !entity->base_state_flag_bits.outside_screen))
            {
                entity->base.unknown_40(area->entities);
                if (entity->base.property_00a_bits.subtype != 8)
                {
                    entity->base.unknown_9c();
                    entity->base.unknown_98(area->first_entity);
                    FieldEntity3D_UpdateSupportClearance(entity);
                    FieldPartyEntity *party = (FieldPartyEntity *)entity;
                    if (entity->base.property_00a_bits.subtype == 0 && party->bits.movement_mode == 6)
                    {
                        func_ov000_020b99b8(party);
                        if (entity->runtime_flag_bits.contact_mask_b & 16)
                        {
                            FieldPartyEntity *partner = party->partner;
                            if (partner && partner->entity.locomotion_state >= 57 &&
                                partner->entity.locomotion_state <= 58)
                                partner->entity.runtime_flags |= FIELD_ENTITY_VERTICAL_SYNC_DIRTY;
                        }
                    }
                    if (entity->base.property_00a_bits.subtype <= 1)
                    {
                        FieldAuxiliaryEntity **slot = party->auxiliaries;
                        for (int i = 0; i < 6; ++i, ++slot)
                        {
                            if (*slot)
                            {
                                ((FieldEntity *)*slot)->unknown_40(area->entities);
                                ((FieldEntity *)*slot)->unknown_98(area->first_entity);
                                ((FieldEntity *)*slot)->unknown_9c();
                            }
                        }
                    }
                }
            }
        }
        for (FieldRuntimeEntity *entity = area->first_entity; entity; entity = entity->base.update_next)
        {
            if (entity->base.property_00a_bits.subtype != 9 && !entity->base.visibility_bits.unknown_08 &&
                (entity->base_state_flag_bits.retain_offscreen_contact ||
                 !entity->base_state_flag_bits.outside_screen))
                entity->base.unknown_44();
        }
        for (FieldRuntimeEntity *entity = area->first_entity; entity; entity = entity->base.update_next)
        {
            if (entity->base.property_00a_bits.subtype != 9 && entity->base.property_00a_bits.subtype != 8 &&
                !entity->base.visibility_bits.unknown_08 &&
                (entity->base_state_flag_bits.retain_offscreen_contact ||
                 !entity->base_state_flag_bits.outside_screen))
                entity->base.update_overlap_priorities(area->flags.unknown_04_05);
        }
        func_ov000_02071ebc(area);
    }
}
