#ifndef PIT_GAME_SAVE_STATE_TRANSFER_H
#define PIT_GAME_SAVE_STATE_TRANSFER_H
#include <game/save_data.h>

/* Stored party records omit derived stats; the live view retains them. */
typedef struct SaveStoredParty {
    u16 base_max_hp, base_power, base_defense, base_speed, base_stache, current_hp;
    SavePartyExperience experience;
    u8 clothing_id, badge_id, unknown_12[2];
} SaveStoredParty;
typedef struct SaveStoredTail {
    SaveStoredParty party[4];
    u32 coins;
    s8 items[14];
    u8 unknown_62[2];
    s8 bros_items[11];
    u8 unknown_6f[5];
    s8 clothing[33];
    u8 unknown_95[17];
    s8 badges[41];
    u8 unknown_cf[9];
    u32 play_time;
    u16 unknown_dc;
    u8 location, unknown_df, unknown_e0, unknown_e1[7];
} SaveStoredTail;
typedef struct SaveLiveTransferView {
    u8 unknown_00[72];
    u8 persistent[944];
    SavePartyMember party[4];
    u32 coins;
    u16 unknown_48c;
    s8 items[14];
    u8 unknown_49c[2];
    s8 bros_items[11];
    u8 unknown_4a9[5];
    s8 clothing[33];
    u8 unknown_4cf[17];
    s8 badges[41];
    u8 unknown_509[11];
    union {
        u8 raw;
        struct {
            u8 unknown_0 : 4, slot : 1, unknown_5 : 1, unknown_6 : 2;
        } bits;
    } slot_select;
    u8 language, unknown_516, unknown_517;
    u32 play_time;
    u8 unknown_51c[68];
    u8 location;
    u8 unknown_561[3];
} SaveLiveTransferView;
typedef char SaveStoredParty_SizeCheck[sizeof(SaveStoredParty) == 20 ? 1 : -1];
typedef char SaveStoredTail_SizeCheck[sizeof(SaveStoredTail) == 232 ? 1 : -1];
typedef char SaveLiveTransferView_SizeCheck[sizeof(SaveLiveTransferView) == 1380 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void SaveState_PackSlot(void);
void SaveState_UnpackSlot(void);
#ifdef __cplusplus
}
#endif
#endif
