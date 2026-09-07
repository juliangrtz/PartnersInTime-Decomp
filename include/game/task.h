#ifndef PIT_GAME_TASK_H
#define PIT_GAME_TASK_H

#include <nitro.h>

typedef struct GameTask GameTask;
typedef struct GameIrqTask GameIrqTask;
typedef struct GameTaskVTable {
    void *destroy;
    void (*delete_task)(void *task);
    void (*update)(void *task);
} GameTaskVTable;

struct GameTask {
    GameTaskVTable *vtable;
    void *argument;
    GameTask *previous;
    GameTask *next;
    u8 status;
    u8 priority;
    u16 active : 1;
    u16 processed : 1;
    s16 interval : 4;
    u16 counter : 4;
    u16 reserved : 6;
    u32 update_count;
};

struct GameIrqTask {
    GameTaskVTable *vtable;
    void *argument;
    GameIrqTask *previous;
    GameIrqTask *next;
    u8 status;
    u8 priority;
    u8 active : 1;
    u8 processed : 1;
    u8 reserved : 6;
    u8 padding;
    u32 update_count;
};

typedef char GameTaskSizeCheck[(sizeof(GameTask) == 24) ? 1 : -1];
typedef char GameIrqTaskSizeCheck[(sizeof(GameIrqTask) == 24) ? 1 : -1];

#ifdef __cplusplus
/* The original task callbacks use the C++ virtual-call ABI. */
class GameTaskDispatch {
public:
    virtual void destroy();
    virtual void delete_task();
    virtual void update();
};
extern "C" {
#endif

void GameTask_InitSystem(void);
void GameTask_UpdateAll(void);
void GameTask_DisableNormal(void);
void GameTask_Enable(GameTask *task);
void GameTask_Disable(GameTask *task);
GameTask *GameTask_DestroyBase(GameTask *task);
GameTask *GameTask_Delete(GameTask *task);
GameTask *GameTask_DestroyComplete(GameTask *task);
GameTask *GameTask_Init(GameTask *task, u32 priority, u32 unused, void *argument);

void GameIrqTask_DeleteSafe(GameIrqTask *task);
void GameIrqTask_InitSystem(void);
void GameIrqTask_UpdateAll(void);
void GameIrqTask_DisableNormal(void);
void GameIrqTask_Enable(GameIrqTask *task);
void GameIrqTask_Disable(GameIrqTask *task);
GameIrqTask *GameIrqTask_DestroyBase(GameIrqTask *task);
GameIrqTask *GameIrqTask_Delete(GameIrqTask *task);
GameIrqTask *GameIrqTask_DestroyComplete(GameIrqTask *task);
GameIrqTask *GameIrqTask_Init(GameIrqTask *task, u32 priority, u32 unused, void *argument);

#ifdef __cplusplus
}
#endif

#endif
