#include <game/battle_capture_surface.h>
int func_0202cc58(const void *source, void *destination, u32 size);
int func_0202cd2c(const void *source, void *destination, u32 size);

int BattleCaptureSurface_CopyTask(BattleCaptureCopyTask *task) {
    u32 size = task->size;
    void *destination = task->destination;
    const void *source = task->source;

    if (source < destination) {
        return func_0202cd2c(source, destination, size);
    }
    return func_0202cc58(source, destination, size);
}
