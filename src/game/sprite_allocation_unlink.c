#include <game/sprite_output.h>

void GameSpriteAllocation_Unlink(GameSpriteAllocation *allocation)
{
    int screen = allocation->flags.bits.screen;
    if (data_0205a06c[screen] && data_0205a06c[screen] == allocation)
        data_0205a06c[screen] = allocation->next;
    if (data_0205a074[screen] && data_0205a074[screen] == allocation)
        data_0205a074[screen] = allocation->previous;
    if (allocation->previous) allocation->previous->next = allocation->next;
    if (allocation->next) allocation->next->previous = allocation->previous;
    allocation->next = 0;
    allocation->previous = allocation->next;
    allocation->flags.raw &= ~0x20;
}
