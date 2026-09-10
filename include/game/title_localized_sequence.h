#ifndef PIT_TITLE_LOCALIZED_SEQUENCE_H
#define PIT_TITLE_LOCALIZED_SEQUENCE_H
#include <game/title_sprite_sequence.h>
typedef struct TitleLocalizedSequence {
    TitleSpriteHeader header;
    TitleTextureResource textures[5];
    const s16 *commands[2];
    GameSpriteAnimationTrack *tracks[2];
    GameSpriteAnimation *animation;
    GameSpriteAnimationTrack track_storage[4];
    u8 animation_buffer[1024];
    u16 rotation, pulse_angle;
    s32 start_scale, acceleration, velocity, vertical_offset;
    union {
        u8 mask;
        struct {
            u8 part_0 : 1, part_1 : 1, part_2 : 1, part_3 : 1, unknown_4_7 : 4;
        } bits;
    } visible_parts;
    u8 unknown_665[3];
    s32 active_elapsed;
} TitleLocalizedSequence;
typedef char TitleLocalizedSequence_SizeCheck[sizeof(TitleLocalizedSequence) == 1644 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
/* The draw callback also advances the localized animation. */
void TitleLocalizedSequence_Draw(TitleLocalizedSequence *work);
void TitleLocalizedSequence_Finish(TitleLocalizedSequence *work);
void TitleLocalizedSequence_Start(TitleLocalizedSequence *work);
void TitleLocalizedSequence_Release(TitleLocalizedSequence *work);
void TitleLocalizedSequence_Init(TitleLocalizedSequence *work);
#ifdef __cplusplus
}
#endif
#endif
