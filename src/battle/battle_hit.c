#include <game/battle_ai.h>
#include <game/battle_hit.h>

BattleHitDescriptor *BattleHitDescriptor_GetByActorId(u16 actor_id) {
    if (actor_id < 0x0A) {
        actor_id = (u16)(actor_id + 0x14);
    } else if (actor_id < 0x38) {
        actor_id = (u16)(actor_id - 0x28);
    } else {
        actor_id -= 0x28;
    }

    return (BattleHitDescriptor *)(gBattleContext + 0xC8F4) + actor_id;
}

#include <game/battle_actor.h>
#include <game/battle_context.h>
#include <game/battle_damage.h>
#include <game/battle_scene.h>

enum BattleHitQueueOffset {
    BATTLE_CURRENT_ACTOR_ID_OFFSET = 0x20,
    BATTLE_HIT_DESCRIPTOR_HEAD_OFFSET = 0xCAD4,
    BATTLE_HIT_QUEUE_OFFSET = 0xCAD8
};
enum BattleHitQueueConstant {
    BATTLE_HIT_QUEUE_CAPACITY = 8,
    BATTLE_OBJECT_TARGET_FIRST = 40,
    BATTLE_OBJECT_TARGET_END = 56
};
enum BattleHitDescriptorTargetFlag {
    BATTLE_HIT_FLAG_SKIP_MARIO = BATTLE_HIT_FLAG_UNKNOWN_0080,
    BATTLE_HIT_FLAG_SKIP_LUIGI = BATTLE_HIT_FLAG_UNKNOWN_0100
};
int BattleModel_GetAnimationBounds(BattleCollisionBounds *bounds,
                                   BattleModel *model, int animation_index,
                                   int frame_index);

int BattleCollision_GetBounds(BattleCollisionBounds *bounds, u32 actor_id,
                              int animation_index, int frame_index) {
    u32 id = actor_id;

    if (id - BATTLE_ACTOR_PARTY_FIRST <= 1) {
        BattlePartyActor *actor;

        if (animation_index > 0) {
            return 0;
        }
        actor = (BattlePartyActor *)BattleActor_GetPartySlot((u16)actor_id);
        switch (actor->formation_index) {
        case BATTLE_FORMATION_MARIO:
            bounds->axis[0] = -3;
            bounds->axis[1] = 6;
            bounds->axis[2] = 30;
            bounds->axis[3] = 8;
            bounds->axis[4] = 0;
            bounds->axis[5] = -4;
            break;
        case BATTLE_FORMATION_LUIGI:
            bounds->axis[0] = -3;
            bounds->axis[1] = 6;
            bounds->axis[2] = 30;
            bounds->axis[3] = 8;
            bounds->axis[4] = 0;
            bounds->axis[5] = -4;
            break;
        case BATTLE_FORMATION_BABY_MARIO:
            bounds->axis[0] = -3;
            bounds->axis[1] = 3;
            bounds->axis[2] = 21;
            bounds->axis[3] = 6;
            bounds->axis[4] = 0;
            bounds->axis[5] = -4;
            break;
        case BATTLE_FORMATION_BABY_LUIGI:
            bounds->axis[0] = -3;
            bounds->axis[1] = 3;
            bounds->axis[2] = 21;
            bounds->axis[3] = 6;
            bounds->axis[4] = 0;
            bounds->axis[5] = -4;
            break;
        case BATTLE_FORMATION_MARIO_CARRYING:
            bounds->axis[0] = -3;
            bounds->axis[1] = 6;
            bounds->axis[2] = 32;
            bounds->axis[3] = 8;
            bounds->axis[4] = 0;
            bounds->axis[5] = -4;
            break;
        case BATTLE_FORMATION_LUIGI_CARRYING:
            bounds->axis[0] = -3;
            bounds->axis[1] = 6;
            bounds->axis[2] = 32;
            bounds->axis[3] = 8;
            bounds->axis[4] = 0;
            bounds->axis[5] = -4;
            break;
        }
    } else if (id - 8 <= 1) {
        if (animation_index > 0) {
            return 0;
        }
        bounds->axis[0] = -8;
        bounds->axis[1] = 6;
        bounds->axis[2] = 10;
        bounds->axis[3] = -10;
        bounds->axis[4] = 4;
        bounds->axis[5] = -4;
    } else {
        if (id >= BATTLE_ACTOR_ENEMY_FIRST) {
            BattleActor_GetById((u16)actor_id);
        }
        {
            BattleSceneObject *object = BattleSceneObject_GetById((u16)id);
            BattleModel *model = BattleSceneObject_GetActiveModel(object);
            int first_axis;

            if (model == 0) {
                return 0;
            }
            if (!BattleModel_GetAnimationBounds(bounds, model, animation_index,
                                                frame_index)) {
                return 0;
            }
            first_axis = bounds->axis[1];
            bounds->axis[1] = bounds->axis[2];
            bounds->axis[2] = -first_axis;
            bounds->axis[3] = -bounds->axis[3];
            bounds->axis[4] = 0;
            bounds->axis[5] = -4;
        }
    }
    return 1;
}

void BattleHitQueue_Update(void) {
    int index = 0;
    BattleHitRecord *previous_record;
    int byte_offset = 0;
    BattleHitRecord *record;
    BattleHitDescriptor *descriptor;
    BattleHitDescriptor *previous;

    do {
        previous_record =
            (BattleHitRecord *)(gBattleContext + BATTLE_HIT_QUEUE_OFFSET +
                                byte_offset);
        if (previous_record->kind == 0) {
            break;
        }
        if (previous_record->source_id != 0) {
            if (previous_record->callback != 0) {
                previous_record->callback(previous_record);
            }
            previous_record->kind = 0;
        }
        ++index;
        byte_offset += sizeof(BattleHitRecord);
    } while (index < BATTLE_HIT_QUEUE_CAPACITY);

    record = (BattleHitRecord *)(gBattleContext + BATTLE_HIT_QUEUE_OFFSET);
    record->kind = 0;
    descriptor = *(BattleHitDescriptor **)(gBattleContext +
                                           BATTLE_HIT_DESCRIPTOR_HEAD_OFFSET);
    previous = 0;

    while (descriptor != 0) {
        if (descriptor->callback != 0) {
            u32 target_id = descriptor->target_id;

            if (target_id == BATTLE_HIT_TARGET_PARTY_PAIR) {
                BattlePartyActor *party =
                    (BattlePartyActor *)BattleActor_GetPartySlot(
                        BATTLE_ACTOR_MARIO);

                if (party->actor.current_hp > 0 &&
                    !party->actor.flag_bits.hit_locked &&
                    !descriptor->flag_bits.skip_mario) {
                    record->kind = BattleCollision_TestObjects(
                        descriptor->source_id, BATTLE_ACTOR_MARIO,
                        (BattlePosition *)&record->x);
                    if ((record->kind & descriptor->flag_bits.hit_kind) != 0) {
                        record->source_id = descriptor->source_id;
                        record->target_id = BATTLE_ACTOR_MARIO;
                        record->status_id = descriptor->flag_bits.status_id;
                        record->status_chance = descriptor->status_chance;
                        record->status_magnitude = descriptor->status_magnitude;
                        record->callback = descriptor->callback;
                        ++record;
                    } else {
                        record->kind = 0;
                    }
                }

                party = (BattlePartyActor *)BattleActor_GetPartySlot(
                    BATTLE_ACTOR_LUIGI);
                if (party->actor.current_hp > 0 &&
                    !party->actor.flag_bits.hit_locked &&
                    !descriptor->flag_bits.skip_luigi) {
                    record->kind = BattleCollision_TestObjects(
                        descriptor->source_id, BATTLE_ACTOR_LUIGI,
                        (BattlePosition *)&record->x);
                    if ((record->kind & descriptor->flag_bits.hit_kind) != 0) {
                        record->source_id = descriptor->source_id;
                        record->target_id = BATTLE_ACTOR_LUIGI;
                        record->status_id = descriptor->flag_bits.status_id;
                        record->status_chance = descriptor->status_chance;
                        record->status_magnitude = descriptor->status_magnitude;
                        record->callback = descriptor->callback;
                        ++record;
                    } else {
                        record->kind = 0;
                    }
                }
            } else if (target_id == BATTLE_HIT_TARGET_ALL) {
                int target;

                for (target = BATTLE_OBJECT_TARGET_FIRST;
                     target < BATTLE_OBJECT_TARGET_END; ++target) {
                    if (BattleHitDescriptor_GetByActorId((u16)target)
                            ->callback != 0) {
                        record->kind = BattleCollision_TestObjects(
                            descriptor->source_id, target,
                            (BattlePosition *)&record->x);
                        if ((record->kind & descriptor->flag_bits.hit_kind) !=
                            0) {
                            record->source_id = descriptor->source_id;
                            record->target_id = target;
                            record->status_id = descriptor->flag_bits.status_id;
                            record->status_chance = descriptor->status_chance;
                            record->status_magnitude =
                                descriptor->status_magnitude;
                            record->callback = descriptor->callback;
                            ++record;
                        } else {
                            record->kind = 0;
                        }
                    }
                }

                for (target = BATTLE_ACTOR_ENEMY_FIRST;
                     target <
                     BATTLE_ACTOR_ENEMY_FIRST + BATTLE_ACTOR_ENEMY_COUNT;
                     ++target) {
                    if (BattleHitDescriptor_GetByActorId((u16)target)
                            ->callback != 0) {
                        record->kind = BattleCollision_TestObjects(
                            descriptor->source_id, target,
                            (BattlePosition *)&record->x);
                        if ((record->kind & descriptor->flag_bits.hit_kind) !=
                            0) {
                            record->source_id = descriptor->source_id;
                            record->target_id = target;
                            record->status_id = descriptor->flag_bits.status_id;
                            record->status_chance = descriptor->status_chance;
                            record->status_magnitude =
                                descriptor->status_magnitude;
                            record->callback = descriptor->callback;
                            ++record;
                        } else {
                            record->kind = 0;
                        }
                    }
                }
            } else {
                if (!((target_id >= BATTLE_ACTOR_ENEMY_FIRST &&
                       target_id < BATTLE_ACTOR_ENEMY_FIRST +
                                       BATTLE_ACTOR_ENEMY_COUNT &&
                       BattleActor_GetById((u16)target_id)
                           ->flag_bits.hit_locked) ||
                      (target_id >= BATTLE_ACTOR_PARTY_FIRST &&
                       target_id < BATTLE_ACTOR_PARTY_FIRST +
                                       BATTLE_ACTOR_PARTY_COUNT &&
                       BattleActor_GetById((u16)target_id)
                           ->flag_bits.hit_locked))) {
                    record->kind = BattleCollision_TestObjects(
                        descriptor->source_id, target_id,
                        (BattlePosition *)&record->x);
                    if ((record->kind & descriptor->flag_bits.hit_kind) != 0) {
                        record->source_id = descriptor->source_id;
                        record->target_id = target_id;
                        record->status_id = descriptor->flag_bits.status_id;
                        record->status_chance = descriptor->status_chance;
                        record->status_magnitude = descriptor->status_magnitude;
                        record->callback = descriptor->callback;
                        ++record;
                        BattleSceneObject_GetById((u16)target_id)
                            ->linked_actor_id = target_id;
                    } else {
                        record->kind = 0;
                    }
                }
            }
            previous = descriptor;
        } else {
            if (previous != 0) {
                previous->next = descriptor->next;
            } else {
                *(BattleHitDescriptor **)(gBattleContext +
                                          BATTLE_HIT_DESCRIPTOR_HEAD_OFFSET) =
                    descriptor->next;
            }
            descriptor->flags &= ~BATTLE_HIT_FLAG_ACTIVE;
        }
        descriptor = descriptor->next;
    }

    {
        int outer_index;
        BattleHitRecord *outer;

        /* Reuse the exhausted descriptor cursor as the fixed queue base. */
        descriptor =
            (BattleHitDescriptor *)(gBattleContext + BATTLE_HIT_QUEUE_OFFSET);
        outer = (BattleHitRecord *)descriptor;

        for (outer_index = 0; outer_index < BATTLE_HIT_QUEUE_CAPACITY;
             ++outer_index, ++outer) {
            int inner_index;
            BattleHitRecord *inner;

            if (outer->kind == 0) {
                break;
            }
            if ((u16)(outer->source_id + (u16)-BATTLE_ACTOR_MARIO) > 1) {
                continue;
            }
            inner_index = 0;
            inner = (BattleHitRecord *)descriptor;
            for (; inner_index < BATTLE_HIT_QUEUE_CAPACITY;
                 ++inner_index, ++inner) {
                if (inner->kind == 0) {
                    break;
                }
                if (inner->source_id == outer->target_id) {
                    inner->source_id = 0;
                    inner->target_id = 0;
                    inner->callback = 0;
                }
            }
        }

        {
            int chain_index;
            int linked_index;
            BattleHitRecord *chain;
            BattleHitRecord *linked;

            chain = (BattleHitRecord *)descriptor;
            for (chain_index = 0; chain_index < BATTLE_HIT_QUEUE_CAPACITY;
                 ++chain_index, ++chain) {
                if (chain->kind == 0) {
                    break;
                }
                if (chain->source_id == 0) {
                    continue;
                }
                linked_index = 0;
                linked = (BattleHitRecord *)descriptor;
                for (; linked_index < BATTLE_HIT_QUEUE_CAPACITY;
                     ++linked_index, ++linked) {
                    if (linked->kind == 0) {
                        break;
                    }
                    if (linked->source_id == chain->target_id) {
                        linked->source_id = 0;
                        linked->target_id = 0;
                        linked->callback = 0;
                    }
                }
            }
        }

        {
            int outer_index;
            int inner_index;
            BattleHitRecord *outer;
            BattleHitRecord *inner;

            outer = (BattleHitRecord *)descriptor;
            for (outer_index = 0; outer_index < BATTLE_HIT_QUEUE_CAPACITY;
                 ++outer_index, ++outer) {
                if (outer->kind == 0) {
                    break;
                }
                if (outer->source_id == 0) {
                    continue;
                }
                inner_index = outer_index + 1;
                if (inner_index >= BATTLE_HIT_QUEUE_CAPACITY) {
                    continue;
                }
                inner = (BattleHitRecord *)descriptor + inner_index;
                do {
                    if (inner->kind == 0) {
                        break;
                    }
                    if (inner->target_id == outer->target_id) {
                        inner->source_id = 0;
                        inner->target_id = 0;
                        inner->callback = 0;
                    }
                    ++inner_index;
                    ++inner;
                } while (inner_index < BATTLE_HIT_QUEUE_CAPACITY);
            }
        }

        {
            int damage_index;

            record = (BattleHitRecord *)descriptor;
            for (damage_index = 0; damage_index < BATTLE_HIT_QUEUE_CAPACITY;
                 ++damage_index, ++record) {
                u32 source_actor_id;
                u32 target_actor_id;
                int damage;

                if (record->kind == 0) {
                    return;
                }
                if (record->source_id == 0) {
                    continue;
                }
                source_actor_id = BattleSceneObject_GetById(record->source_id)
                                      ->linked_actor_id;
                target_actor_id = BattleSceneObject_GetById(record->target_id)
                                      ->linked_actor_id;

                if ((target_actor_id >= BATTLE_ACTOR_PARTY_FIRST &&
                     target_actor_id <
                         BATTLE_ACTOR_PARTY_FIRST + BATTLE_ACTOR_PARTY_COUNT) ||
                    (target_actor_id >= BATTLE_ACTOR_ENEMY_FIRST &&
                     target_actor_id <
                         BATTLE_ACTOR_ENEMY_FIRST + BATTLE_ACTOR_ENEMY_COUNT)) {
                    u32 current_actor_id = *(u16 *)(
                        gBattleContext + BATTLE_CURRENT_ACTOR_ID_OFFSET);

                    if (current_actor_id < BATTLE_ACTOR_PARTY_FIRST ||
                        current_actor_id >= BATTLE_ACTOR_PARTY_FIRST +
                                                BATTLE_ACTOR_PARTY_COUNT) {
                        damage = BattleDamage_CalculateByObject(
                            record->source_id, record->target_id);
                        if (damage >= 0) {
                            BattleActor_GetById(target_actor_id)
                                ->pending_damage = damage;
                        }
                        if (damage < 0) {
                            u16 source_id;

                            BattleActor_GetById(source_actor_id)
                                ->pending_damage = -damage;
                            source_id = record->source_id;
                            record->source_id = record->target_id;
                            record->target_id = source_id;
                            target_actor_id = source_actor_id;
                        }
                    }
                }

                if (target_actor_id >= BATTLE_ACTOR_ENEMY_FIRST &&
                    target_actor_id <
                        BATTLE_ACTOR_ENEMY_FIRST + BATTLE_ACTOR_ENEMY_COUNT) {
                    u32 current_actor_id = *(u16 *)(
                        gBattleContext + BATTLE_CURRENT_ACTOR_ID_OFFSET);

                    if (current_actor_id >= BATTLE_ACTOR_ENEMY_FIRST &&
                        current_actor_id < BATTLE_ACTOR_ENEMY_FIRST +
                                               BATTLE_ACTOR_ENEMY_COUNT) {
                        BattleAI_StartReactionScript(target_actor_id);
                    }
                    {
                        u32 flags;
                        u8 *runtime_base;

                        runtime_base = gBattleContext;
                        runtime_base += BATTLE_RUNTIME_STATE_OFFSET;
                        flags = *(u32 *)(runtime_base + 0x3A0);
                        flags &= ~0x1000;
                        *(u32 *)(runtime_base + 0x3A0) = flags;
                    }
                }
            }
        }
    }
}

enum {
    BATTLE_EFFECT_VARIANT_OFFSET = 0xCB7A
};
BattleHitDescriptor *BattleHitDescriptor_Configure(
    u16 source_id, u16 target_id, BattleHitCallback callback,
    u16 linked_actor_id, int hit_kind) {
    BattleHitDescriptor *descriptor =
        BattleHitDescriptor_GetByActorId(source_id);

    descriptor->flags &= ~BATTLE_HIT_STATUS_MASK;
    if (target_id >= 0xFFFF) {
        descriptor->callback = 0;
        return descriptor;
    }

    if (((u32)(descriptor->flags << 25) >> 31) == 0) {
        u8 *volatile *context_slot = &gBattleContext;

        descriptor->flags |= BATTLE_HIT_FLAG_ACTIVE;
        descriptor->next = *(BattleHitDescriptor **)(*context_slot + 0xCAD4);
        *(BattleHitDescriptor **)(*context_slot + 0xCAD4) = descriptor;
    }

    {
        int effective_hit_kind = hit_kind;
        u16 flags;

        descriptor->source_id = source_id;
        descriptor->target_id = target_id;
        if (effective_hit_kind == 0) {
            effective_hit_kind = BATTLE_HIT_KIND_DEFAULT;
        }
        flags = descriptor->flags;
        effective_hit_kind = (u16)effective_hit_kind;
        if (callback == 0) {
            callback = BattleDamage_DispatchHit;
        }
        flags = (flags & ~BATTLE_HIT_KIND_MASK) |
                (effective_hit_kind & BATTLE_HIT_KIND_MASK);
        descriptor->flags = flags;
    }
    descriptor->callback = callback;
    descriptor->flags &= ~BATTLE_HIT_FLAG_UNKNOWN_0080;
    descriptor->flags &= ~BATTLE_HIT_FLAG_UNKNOWN_0100;

    if (linked_actor_id == 0) {
        linked_actor_id = source_id;
    }
    BattleSceneObject_GetById(source_id)->linked_actor_id = linked_actor_id;
    return descriptor;
}

void BattleHitDescriptor_SetStatus(BattleHitDescriptor *descriptor,
                                   int status_id, int chance, int magnitude) {
    u16 status = (u16)status_id;

    descriptor->flags =
        (descriptor->flags & ~BATTLE_HIT_STATUS_MASK) |
        ((status & (BATTLE_HIT_STATUS_MASK >> BATTLE_HIT_STATUS_SHIFT))
         << BATTLE_HIT_STATUS_SHIFT);
    descriptor->status_chance = chance;
    descriptor->status_magnitude = magnitude;
}

void BattleHitDescriptor_DisableByActor(int actor_id) {
    BattleHitDescriptor_Disable(
        BattleHitDescriptor_GetByActorId((u16)actor_id));
}

void BattleHitDescriptor_Disable(BattleHitDescriptor *descriptor) {
    descriptor->callback = 0;
}

void BattleDamage_ReflectQueuedHits(int target_actor_id) {
    int index = 0;
    BattleHitRecord *record =
        (BattleHitRecord *)(gBattleContext + BATTLE_HIT_QUEUE_OFFSET);

    do {
        int damage;

        if (record->kind == 0) {
            return;
        }
        if (target_actor_id == record->target_id) {
            damage = BattleDamage_CalculateByObject(record->target_id,
                                                    record->source_id);
            if (damage > 0) {
                BattleSceneObject *source =
                    BattleSceneObject_GetById(record->source_id);
                BattleActor_GetById(source->linked_actor_id)->pending_damage =
                    damage;
            }

            {
                u16 target_id = record->target_id;
                BattleHitDescriptor *descriptor;

                record->target_id = record->source_id;
                record->source_id = target_id;
                descriptor = BattleHitDescriptor_GetByActorId(target_id);
                record->status_id =
                    (u32)(descriptor->flags << 16) >>
                    (16 + BATTLE_HIT_STATUS_SHIFT);
                record->status_chance = descriptor->status_chance;
                record->status_magnitude = descriptor->status_magnitude;
            }
        }

        ++index;
        ++record;
    } while (index < BATTLE_HIT_QUEUE_CAPACITY);
}

void BattleEffect_SetVariant(int variant) {
    *(s16 *)(gBattleContext + BATTLE_EFFECT_VARIANT_OFFSET) = variant;
}
