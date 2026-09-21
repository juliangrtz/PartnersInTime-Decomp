/* VBlank field graphics: pending palette backups, dirty mask rows, both
 * areas, and finally OAM submission and clearing for the next frame. */
#include <game/field_tasks.h>
#include <game/field_system.h>
#include <game/input.h>
#include <game/field_display.h>
#include <game/field_area.h>
extern GameFrameTiming data_02060b2c;
void func_ov000_02066028(FieldSystem *);
void FieldGraphicsIrqTask_Update(FieldGraphicsIrqTask *task)
{
    if (data_02060b2c.bits.waiting_vblank) {
        FieldSystem *system = task->task.argument;
        if (system->task.status <= 2U) {
            int i;
            if (task->flags.pending) {
                FieldDisplay_SavePalettes(system);
                task->flags.pending = 0;
            }
            func_ov000_02066028(system);
            for (i = 0; i < 2; ++i) {
                if (system->areas[i]) FieldArea_UpdateGraphics(system->areas[i]);
            }
        }
        GameDisplay_CopyOam(3);
        GameDisplay_ClearOam();
    }
}
