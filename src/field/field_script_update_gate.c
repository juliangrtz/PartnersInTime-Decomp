/*
 * Script update gate (overlay 0, 0x0207EB28-0x0207EB90).
 *
 * Runs the area's scripts only while nothing else holds control.
 */

#include <game/field_area.h>
extern void func_ov000_02088c88(FieldScriptManager *);
typedef struct FieldOwnerModeView {
    u8 prefix[600];
    u16 mode : 4, reserved : 12;
} FieldOwnerModeView;

void FieldArea_UpdateScriptsWhenIdle(FieldAreaContext *area) {
    if ((!area->auxiliary || !((FieldOwnerModeView *)area->auxiliary)->mode) &&
        area->unknown_23f0 == 71)
        func_ov000_02088c88(&area->scripts);
}
