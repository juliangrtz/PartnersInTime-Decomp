/*
 * OAM state copy (ARM9 resident, 0x020077F0-0x02007848).
 *
 * Copies the current OAM state out for a caller that wants to restore it.
 */

#include <game/sprite_output.h>

void GameOam_CopyState(void *destination)
{
    if (data_0205a054) GameSprite_CopyBytes(data_0205a054, destination, 804);
}
