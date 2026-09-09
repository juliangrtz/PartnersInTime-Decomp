#ifndef PIT_GAME_FIELD_ROOM_TRANSITION_H
#define PIT_GAME_FIELD_ROOM_TRANSITION_H
#include <nitro.h>

struct FieldAreaContext;

/* Resident room metadata: the second halfword links the paired screen's room. */
typedef struct FieldRoomDescriptor {
    u16 music : 14, type : 2;
    u16 paired_room;
} FieldRoomDescriptor;
typedef char FieldRoomDescriptor_SizeCheck[sizeof(FieldRoomDescriptor) == 4 ? 1 : -1];

typedef struct FieldMusicActivationFlags {
    u8 disabled : 1, unknown_01_07 : 7;
} FieldMusicActivationFlags;
typedef char FieldMusicActivationFlags_SizeCheck[sizeof(FieldMusicActivationFlags) == 1 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
extern const FieldRoomDescriptor data_02048f18[];
void FieldArea_PrepareRoomDeparture(struct FieldAreaContext *field);
void FieldArea_QueueRoomChange(struct FieldAreaContext *field, int room, s16 script, int bgm, u8 fade);
void FieldArea_QueueBothPartyRoomChange(struct FieldAreaContext *field, int room, s16 x0, s16 y0, s16 z0,
                                        int direction0, s16 x1, s16 y1, s16 z1, int direction1, s16 script,
                                        int bgm, u8 fade, u8 synchronize, s16 paired_script, u8 paired_fade);
void FieldArea_QueuePartyRoomChange(struct FieldAreaContext *field, int side, int room, s16 x, s16 y, s16 z,
                                    int direction, u8 lock_control, s16 script, int bgm, u8 fade,
                                    u8 synchronize, s16 paired_script, u8 paired_fade);
#ifdef __cplusplus
}
#endif
#endif
