/*
 * Link effect simulation (overlay 2, 0x020BE30C-0x020BE3B0).
 *
 * Advances a link effect's simulation and draws the result.
 */

#include <game/battle_link_effect.h>
void func_ov002_020be060(BattleLinkPoint *points, unsigned count, int end_mass, int spacing);
void func_ov002_020bdc50(BattleLinkPoint *points, unsigned count, int depth, int screen_y,
                         u16 color, u8 width);
void BattleLinkEffect_SimulateAndDraw(BattleLinkPoint *points, unsigned count, int spacing,
                                      int end_mass, int depth, int screen_y, u16 color, u8 width) {
    /* Each frame performs five solver steps before drawing. */
    func_ov002_020be060(points, count, end_mass, spacing);
    func_ov002_020be060(points, count, end_mass, spacing);
    func_ov002_020be060(points, count, end_mass, spacing);
    func_ov002_020be060(points, count, end_mass, spacing);
    func_ov002_020be060(points, count, end_mass, spacing);
    func_ov002_020bdc50(points, count, depth, screen_y, color, width);
}
