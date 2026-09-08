#include <game/field_system.h>
#include <game/heap.h>
#include <game/rumble.h>
#include <game/save_data.h>
extern GameTaskVTable data_ov000_020c0de4;
extern FieldPersistentSystems data_0205a01c;
extern const GameRumblePattern data_ov000_020bf9b0[];
extern void func_0202cbd4(void *, int, u32);
extern void func_ov000_0206dc84(FieldSystem *);
extern void func_ov000_0206dba4(FieldSystem *);
extern GameTask *func_ov000_02065f30(GameTask *, u32, u32, void *);
extern GameIrqTask *func_ov000_02065e34(GameIrqTask *, u32, u32, void *);
extern void VBlankIntrWait(void);
FieldSystem *FieldSystem_Init(FieldSystem *system, u32 priority, u32 unused, void *argument)
{
    FieldTimer *timer;
    FieldModelAnimation *models;
    FieldPartyManager *parties;
    FieldArchive *archive;
    GameTask *update;
    GameIrqTask *irq;
    FieldSpriteAnimation *sprites;
    GameTask_Init(&system->task, priority, unused, argument);
    system->task.vtable = &data_ov000_020c0de4;
    func_0202cbd4(system->storage, 0, sizeof(system->storage));
    gSaveData[0x514] |= 0x20;
    GameRumble_SetPatterns(data_ov000_020bf9b0, 8);
    func_ov000_0206dc84(system);
    func_ov000_0206dba4(system);
    if (!data_0205a01c.flags.initialized) {
        timer = GameHeap_New(sizeof(FieldTimer), 1, 0, 0);
        if (timer)
            timer = FieldTimer_Init(timer);
        system->timer = timer;
        models = GameHeap_New(sizeof(FieldModelAnimation), 1, 0, 0);
        if (models)
            models = FieldModelAnimation_Init(models);
        system->model_animation = models;
        parties = GameHeap_New(sizeof(FieldPartyManager), 1, 0, 0);
        if (parties)
            parties = FieldPartyManager_InitializeStorage(parties);
        system->party_manager = parties;
    } else {
        system->timer = data_0205a01c.timer;
        system->model_animation = data_0205a01c.model_animation;
        system->party_manager = data_0205a01c.party_manager;
    }
    DisplayVram_ClearEngineAssignments(0);
    DisplayVram_ClearEngineAssignments(1);
    archive = GameHeap_New(sizeof(FieldArchive), 0, 0, 0);
    if (archive)
        archive = FieldArchive_Init(archive, 10, 0, system, -1);
    system->archive = archive;
    update = GameHeap_New(40, 0, 0, 0);
    if (update)
        update = func_ov000_02065f30(update, 11, 0, system);
    system->update_task = update;
    irq = GameHeap_New(44, 0, 0, 0);
    if (irq)
        irq = func_ov000_02065e34(irq, 8, 0, system);
    system->irq_task = irq;
    sprites = GameHeap_New(sizeof(FieldSpriteAnimation), 0, 0, 0);
    if (sprites)
        sprites = FieldSpriteAnimation_Init(sprites, system);
    system->sprite_animation = sprites;
    GameIrqTask_Enable(system->irq_task);
    VBlankIntrWait();
    return system;
}
FieldSystem *FieldSystem_Destroy(FieldSystem *system)
{
    system->task.vtable = &data_ov000_020c0de4;
    GameTask_DestroyBase(&system->task);
    return system;
}
FieldSystem *FieldSystem_Delete(FieldSystem *system)
{
    system->task.vtable = &data_ov000_020c0de4;
    GameTask_DestroyBase(&system->task);
    GameHeap_Delete(system);
    return system;
}
void FieldSystem_UpdateTouch(FieldSystem *system)
{
    if (system->touch.enabled) {
        system->touch.previous_touch = system->touch_sample.touch;
        TP_RequestSamplingAsync();
        TP_WaitCalibratedResult(&system->touch_sample);
        if (system->touch_sample.touch) {
            if (!system->touch_sample.validity) {
                system->touch.state = !system->touch.previous_touch ? FIELD_TOUCH_PRESSED : FIELD_TOUCH_HELD;
                system->touch.x = system->touch_sample.x;
                system->touch.y = system->touch_sample.y;
            } else if (system->touch.x != -1) {
                system->touch.state = !system->touch.previous_touch ? FIELD_TOUCH_PRESSED : FIELD_TOUCH_HELD;
            } else {
                system->touch.state = FIELD_TOUCH_IDLE;
                system->touch.x = -1;
                system->touch.y = -1;
            }
        } else if (system->touch.previous_touch) {
            system->touch.state = FIELD_TOUCH_RELEASED;
        } else {
            system->touch.state = FIELD_TOUCH_IDLE;
            system->touch.x = -1;
            system->touch.y = -1;
        }
    } else {
        system->touch.state = FIELD_TOUCH_DISABLED;
        system->touch.x = -1;
        system->touch.y = -1;
    }
}
void FieldSystem_SaveSnapshot(FieldSystem *system, FieldSystemSnapshot *snapshot,
                              const FieldAreaContext *first, const FieldAreaContext *second)
{
    FieldPartyManager_SaveSnapshot(system->party_manager, &snapshot->parties);
    FieldArea_CaptureSnapshot(first, &snapshot->areas[0]);
    FieldArea_CaptureSnapshot(second, &snapshot->areas[1]);
}
