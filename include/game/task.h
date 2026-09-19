#ifndef PIT_GAME_TASK_H
#define PIT_GAME_TASK_H

#include <nitro.h>

/* The game runs two scheduler lists. Normal tasks are stepped once per frame
   from the main loop (GameTask_UpdateAll); IRQ tasks are stepped from the
   VBlank handler and do the work that has to hit the hardware inside the
   blanking period. Both lists are kept sorted by ascending priority, and a
   task links itself in when it is initialized.

   Everything on a list is an object whose first word is a dispatch table, so a
   scene subclasses the task rather than registering a callback. */

typedef struct GameTask GameTask;
typedef struct GameIrqTask GameIrqTask;

/* The three slots every task object starts with. `destroy` is the compiler's
   destructor slot; `delete_task` unlinks and frees; `update` is one step. */
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
    /* Position in the list. 0 and 255 are pinned: GameTask_DisableNormal
       leaves those two running and stops everything in between. */
    u8 priority;
    u16 active : 1;
    /* Set once the task has been stepped this frame. The update pass restarts
       from the head whenever a task is still incomplete, so a task added or
       removed mid-pass is handled without walking a stale list. */
    u16 processed : 1;
    /* How often the task runs, counted in `counter`. A positive interval runs
       the task that many times per frame; a negative one (bit 3 set, so the
       value reads as negative in four bits) runs it once every -interval
       frames. */
    s16 interval : 4;
    u16 counter : 4;
    u16 reserved : 6;
    u32 update_count;
};

/* Same layout, but the interval fields are absent: an IRQ task runs exactly
   once per VBlank. */
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

/* Each of these allocates the scene's task, installs its dispatch table and
   links it into the normal list; the scene then runs until it deletes itself. */
GameTask *GameTask_CreateTitleScreen(void);
GameTask *GameTask_CreateSceneController(void);
GameTask *GameTask_CreateSaveMenu(void);
GameTask *GameTask_CreateLoadMenu(void);
GameTask *GameTask_CreateGameOver(void);
GameTask *GameTask_CreateShopMenu(void);
GameTask *GameTask_CreateStaffCredits(void);
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
