#ifndef PARTNERS_IN_TIME_OVERLAY018_ATTACK_H
#define PARTNERS_IN_TIME_OVERLAY018_ATTACK_H

#include <nitro.h>

#ifdef __cplusplus
extern "C" {
#endif

int Overlay18Attack_GetTerminalVelocityParameter(int progress, int grade);
int Overlay18Attack_GetMotionDuration(int progress, int grade);
int Overlay18Attack_GetAlternateActorChance(int progress);
int Overlay18Attack_GetLaunchHeight(int progress);
const s16 *Overlay18Attack_GetInputTimingWindow(int progress, int grade);
int Overlay18Attack_GetHorizontalDistance(int progress, int grade);
int Overlay18Attack_GetTravelSpeedQ8(int progress);
int Overlay18Attack_GetMotionScaleQ8(int progress);
const s16 *Overlay18Attack_SelectThresholdRow(
    const s16 *table, int row_width, int value);

#ifdef __cplusplus
}
#endif

#endif
