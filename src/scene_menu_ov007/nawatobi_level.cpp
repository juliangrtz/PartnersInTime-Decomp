#include "nawatobi_internal.h"

extern "C" {
int SceneScript_StartPrimary(u8 *manager);
int SceneScript_IsPrimaryRunning(const u8 *manager);
void func_ov005_02066358(NawatobiMenuTask *, void (*)(PauseMenuElement *), int);

void NawatobiLevel_Update(NawatobiMenuTask *task)
{
    switch (task->phase) {
    case NAWATOBI_LEVEL_START:
        ((NawatobiScriptVariablesPrefix *)data_ov007_020a6b90)->variables[0] = data_ov007_020a6bcc[NAWATOBI_STATE_LEVEL];
        SceneScript_StartPrimary(data_ov007_020a6b90);
        data_ov007_020a6bcc[NAWATOBI_STATE_SCRIPT_RUNNING] = 1;
        task->timer = 0;
        ++task->phase;
        break;
    case NAWATOBI_LEVEL_WAIT_SCRIPT:
        if (!SceneScript_IsPrimaryRunning(data_ov007_020a6b90)) {
            data_ov007_020a6bcc[NAWATOBI_STATE_SCRIPT_RUNNING] = 0;
            task->timer = 60;
            ++task->phase;
        }
        break;
    case NAWATOBI_LEVEL_RETURN_DELAY:
        if (task->timer)
            --task->timer;
        else
            func_ov005_02066358(task, func_ov007_0208c5bc, 0);
        break;
    }
}
}
