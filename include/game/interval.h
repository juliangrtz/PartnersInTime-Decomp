#ifndef PIT_GAME_INTERVAL_H
#define PIT_GAME_INTERVAL_H
#ifdef __cplusplus
extern "C" {
#endif
/* Inclusive overlap; each pair may be supplied in either order. */
int GameIntervals_Overlap(int first_start, int first_end, int second_start, int second_end);
#ifdef __cplusplus
}
#endif
#endif
