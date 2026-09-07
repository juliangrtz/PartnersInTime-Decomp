#include <game/texture_allocation.h>
void GameTextureAllocation_Unlink(GameTextureAllocation *allocation)
{
    if (data_0205a8c0 && data_0205a8c0 == allocation) data_0205a8c0 = allocation->next;
    if (data_0205a8b4 && data_0205a8b4 == allocation) data_0205a8b4 = allocation->previous;
    if (allocation->previous) allocation->previous->next = allocation->next;
    if (allocation->next) allocation->next->previous = allocation->previous;
    allocation->previous = allocation->next = 0;
    allocation->flags.raw &= ~8;
}
