#include <game/sprite_output.h>

void GameOam_CopyState(void *destination)
{
    if (data_0205a054) GameSprite_CopyBytes(data_0205a054, destination, 804);
}
