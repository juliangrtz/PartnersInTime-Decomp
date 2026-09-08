#include <game/field_party.h>
FieldPartyEntity *FieldPartyAuxiliary_CopyState(FieldPartyEntity *entity, const FieldPartyEntity *source)
{
    entity->auxiliary_bits.unknown_00 = source->auxiliary_bits.unknown_00;
    entity->auxiliary_bits.unknown_01 = source->auxiliary_bits.unknown_01;
    entity->auxiliary_bits.direction_mode = source->auxiliary_bits.direction_mode;
    entity->auxiliary_bits.unknown_04 = source->auxiliary_bits.unknown_04;
    entity->auxiliary_bits.unknown_05_06 = source->auxiliary_bits.unknown_05_06;
    entity->auxiliary_bits.unknown_07_08 = source->auxiliary_bits.unknown_07_08;
    entity->auxiliary_bits.unknown_09 = source->auxiliary_bits.unknown_09;
    entity->auxiliary_bits.unknown_10_15 = source->auxiliary_bits.unknown_10_15;
    entity->auxiliary_bits.unknown_16_18 = source->auxiliary_bits.unknown_16_18;
    entity->auxiliary_bits.unknown_19 = source->auxiliary_bits.unknown_19;
    entity->auxiliary_bits.unknown_20_27 = source->auxiliary_bits.unknown_20_27;
    entity->auxiliary_bits.unknown_28_31 = source->auxiliary_bits.unknown_28_31;
    entity->auxiliary_motion.unknown_524[0] = source->auxiliary_motion.unknown_524[0];
    entity->auxiliary_motion.unknown_524[1] = source->auxiliary_motion.unknown_524[1];
    entity->auxiliary_motion.unknown_524[2] = source->auxiliary_motion.unknown_524[2];
    entity->auxiliary_motion.unknown_524[3] = source->auxiliary_motion.unknown_524[3];
    entity->auxiliary_motion.unknown_524[4] = source->auxiliary_motion.unknown_524[4];
    entity->auxiliary_motion.unknown_524[5] = source->auxiliary_motion.unknown_524[5];
    entity->auxiliary_motion.unknown_53c = source->auxiliary_motion.unknown_53c;
    entity->auxiliary_motion.unknown_540 = source->auxiliary_motion.unknown_540;
    entity->auxiliary_motion.unknown_544 = source->auxiliary_motion.unknown_544;
    entity->auxiliary_motion.target = source->auxiliary_motion.target;
    FieldEntity_CopySpatialState(&entity->entity, &source->entity);
    return entity;
}
