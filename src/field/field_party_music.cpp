/* Apply a queued music change on the primary field screen. A signed -1
 * leaves playback unchanged; zero requests a stop. Preserve the bank-clear
 * flag, which can still apply when no sequence change is pending. */
#include <game/field_party_manager.h>
#include <game/field_room_transition.h>
extern "C" {
#include <game/audio.h>
#include <game/save_data.h>
void FieldArea_ApplyQueuedMusic(FieldAreaContext *area)
{
    if (!area->flags.screen && !((FieldMusicActivationFlags *)(gSaveData + 1300))->disabled) {
        int music = area->party->bits.queued_bgm;
        if (music != -1) {
            if (music)
                GameAudio_SetMusic(music);
            else
                GameAudio_StopMusic(0);
            area->party->bits.bgm = area->party->bits.queued_bgm;
            area->party->bits.queued_bgm = -1;
        }
        if (area->party->bits.clear_music_bank)
            GameAudio_ClearMusicBank(1);
    }
}
}
