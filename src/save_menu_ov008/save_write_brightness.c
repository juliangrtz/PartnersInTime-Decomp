#include "save_write_effects_internal.h"

void SaveMenuWrite_UpdateBrightness(SaveWriteBrightnessTask *task)
{
    switch (task->phase) {
    case 0:
        func_ov005_0206781c(1, 16);
        task->timer = 12;
        task->amount = 0;
        ++task->phase;
        break;
    case 1:
        if (task->timer) {
            --task->timer;
        } else {
            int amount;
            task->amount += 10922;
            amount = task->amount / 4096;
            if (amount > 16) amount = 16;
            func_ov005_0206781c(1, 16 - amount);
            if (amount >= 16) func_ov005_0206650c((SaveMenuTransferTask *)task);
        }
        break;
    }
}
