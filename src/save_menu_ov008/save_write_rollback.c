/*
 * Save write rollback (overlay 8, 0x0206ABD0-0x0206AC14).
 *
 * Starts the rollback shown when a write fails.
 */

#include "save_write_effects_internal.h"

void SaveMenuWrite_StartRollback(void)
{
    SaveMenuTransferTask *task = func_ov005_0206659c((void (*)(SaveMenuTransferTask *))func_ov008_0206ac14, 2, 1);
    task->arguments[0] = 1;
    task->counter = 0;
    func_ov008_020727d8(0, 6);
}
