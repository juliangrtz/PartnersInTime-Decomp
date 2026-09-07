#include <game/session.h>

void GameSessionTask_RequestState(GameSessionTask *task, u32 requested_state)
{
    task->requested_state = requested_state;
    task->base.status = 0;
    GameTask_Enable(&task->base);
}

void GameSessionTask_RequestStatePhase2(GameSessionTask *task, u32 requested_state)
{
    task->requested_state = requested_state;
    task->base.status = 2;
    GameTask_Enable(&task->base);
}
