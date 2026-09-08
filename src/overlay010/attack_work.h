#ifndef PIT_OVERLAY010_ATTACK_WORK_H
#define PIT_OVERLAY010_ATTACK_WORK_H
extern "C" {
#include <nitro.h>
#include <game/battle_actor.h>
#include <game/battle_damage.h>
#include <game/heap.h>
#include <game/item.h>
#include <game/overlay010_motion.h>
#include <game/save_data.h>
}
#include <game/battle_effect.h>

struct Overlay10ActionActor {
    BattlePartyActor party;
    u16 unknown_84, action_item;
};
struct Overlay10Transition {
    s16 first, second, phase, unknown;
};
struct Overlay10Work {
    int timer, timer2;
    u8 unknown_08[8];
    u8 flags;
    s8 phase;
    u8 unknown_12[6];
    Overlay10Transition transitions[2];
    Overlay10ActionActor *user;
    BattlePartyActor combined_actor;
    u8 unknown_b0[16];
    int *effect_clock;
    int effect_duration;
    u8 effect_shift;
    u8 unknown_c9[3];
    int score;
    u8 enemy_count, unknown_d1;
    u16 enemy_ids[6];
    s16 current_hp[6], position_z[6];
    u16 scale_steps[6], target;
    int parameter, parameter2;
    void (*callback)(Overlay10ActionActor *);
    s16 home[2][2], duration[2];
    u16 sound[2];
    s16 background_x, background_y, background_duration;
    u8 enabled : 1, prompt_animation : 5, background_phase : 2;
    u8 mirror : 1, animation : 7;
    u8 skip_prompt : 1, rating : 3, unknown_128 : 4;
    s8 unknown_129, prompt_x, prompt_y, offset_x, offset_y;
    u8 unknown_12e[2];
};
struct Overlay10TargetFlags {
    u8 uniform : 1, count_index : 2, unknown : 5;
};
struct Overlay10RatingFlags {
    u16 perfect : 1, unknown : 15;
};
struct Overlay10BadgeView {
    u8 unknown[1048];
    u8 badge;
};
struct Overlay10BattleFlags {
    u32 unknown_00 : 12, mode : 1, unknown_13 : 19;
};
typedef char Overlay10WorkSize[sizeof(Overlay10Work) == 304 ? 1 : -1];
#endif
