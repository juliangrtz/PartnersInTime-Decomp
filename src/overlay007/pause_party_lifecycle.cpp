#include <game/overlay007_party.h>
extern "C" {
#include <game/heap.h>

void *PauseParty_Init(void *storage, void *fonts)
{
    Overlay7Party *party = (Overlay7Party *)storage;
    party->list_images = 0;
    GameText_Init(&party->text, (const u32 *const *)fonts, (u8 *)party + sizeof(*party),
                  0, 0, 0, 1, 3, 1, 6, 31, 0, 255, 0);
    return party;
}

void *PauseParty_Destroy(void *storage)
{
    Overlay7Party *party = (Overlay7Party *)storage;
    if (party->list_images) {
        GameHeap_DeleteArray(party->list_images);
        party->list_images = 0;
    }
    return party;
}
}
