/* Forward frame, navigation and renderer lifecycle operations to owned auxiliaries. */
#include <game/field_entity.h>
#include <game/field_party.h>
#include <game/field_auxiliary.h>
#include <game/field_entity_motion.h>
#include <game/field_entity_lifecycle.h>
extern "C" {
void FieldPartyEntity_AllocateRenderers(FieldPartyEntity *party)
{
    FieldEntity3D_AllocateRenderers(&party->entity);
    for (int i = 0; i < 6; ++i)
        if (party->auxiliaries[i])
            party->auxiliaries[i]->entity.base.unknown_28();
}
void FieldPartyEntity_ReleaseRenderers(FieldPartyEntity *party)
{
    FieldEntity3D_ReleaseRenderers(&party->entity);
    for (int i = 0; i < 6; ++i)
        if (party->auxiliaries[i])
            party->auxiliaries[i]->entity.base.release_renderers();
}
void FieldPartyEntity_SetNavigationSurfaces(FieldPartyEntity *party, FieldNavigationSurface *surfaces,
                                           const void *resource)
{
    FieldEntity3D_SetNavigationSurfaces(&party->entity, surfaces, resource);
    for (int i = 0; i < 6; ++i)
        if (party->auxiliaries[i])
            party->auxiliaries[i]->entity.base.set_navigation_surfaces(surfaces, resource);
}
void FieldPartyEntity_BeginFrame(FieldPartyEntity *party)
{
    FieldEntity3D_BeginFrame(&party->entity);
    for (int i = 0; i < 6; ++i)
        if (party->auxiliaries[i])
            party->auxiliaries[i]->entity.base.unknown_08();
}
}
