#ifndef PIT_GAME_SYSTEM_H
#define PIT_GAME_SYSTEM_H
#include <game/input.h>
#include <nitro/tp.h>
#include <nitro/os_owner.h>

typedef struct GameSystemInformation {
    int calibrated_touch;
    TpCalibration touch_calibration;
    OsOwnerInfo owner;
    u8 mac_address[6];
    u8 unknown_62[2];
    u32 owner_value_68[2];
} GameSystemInformation;
typedef char GameSystemInformation_SizeCheck[sizeof(GameSystemInformation) == 108 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
extern GameSystemInformation data_02059f7c;
void GameSystem_Init(void);
int GameSystem_CheckResetKeys(void);
void GameSystem_UpdateVBlank(void);
#ifdef __cplusplus
}
#endif
#endif
