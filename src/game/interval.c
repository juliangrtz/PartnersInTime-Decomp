/*
 * Interval overlap (ARM9 resident, 0x020108F0-0x02010960).
 *
 * Whether two closed intervals intersect, the primitive several collision tests
 * are built on.
 */

#include <game/interval.h>
int GameIntervals_Overlap(int a0, int a1, int b0, int b1) {
    if (a0 > a1) {
        int swap = a1;
        a1 = a0;
        a0 = swap;
    }
    if (b0 > b1) {
        int swap = b1;
        b1 = b0;
        b0 = swap;
    }
    return (a0 <= b0 && b0 <= a1) || (a0 <= b1 && b1 <= a1) || (b0 <= a0 && a0 <= b1) ||
           (b0 <= a1 && a1 <= b1);
}
