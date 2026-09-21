/* Draw the primary model through the renderer that saves/restores its state. */
#include <game/battle_impact_effect.h>

void BattleImpactParticle_RenderSavedPrimary(BattleImpactParticlePayload *particle)
{
    BattleImpactParticle_RenderSavedModel(particle, particle->object->primary_model,
                                          particle->x, particle->y, particle->z);
}
