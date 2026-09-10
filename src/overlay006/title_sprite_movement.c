#include <game/title_sprite_sequence.h>

extern const u8 data_ov006_0207b2a4[];
void func_ov006_0206fec4(TitleSequenceSprite *);
void func_ov006_0206fd90(TitleSequenceSprite *);
void func_ov006_02070bd4(s32 *, s32 *, int, int, int);

void TitleSequenceSprite_Init(TitleSequenceSprite *work, TitleTextureResource *texture, int index, int depth,
                              int final_depth)
{
    work->texture = texture;
    work->layout = data_ov006_0207b2a4 + 16 * index;
    work->acceleration = 0;
    work->velocity = 0;
    work->vertical_offset = 0;
    work->scale_x = 0;
    work->scale_y = 0;
    work->alpha = 0;
    work->header.update = (void (*)(void *))func_ov006_0206fec4;
    work->header.draw = (void (*)(void *))func_ov006_0206fd90;
    work->header.x = 0;
    work->header.y = 0;
    work->header.depth = depth;
    work->final_depth = final_depth;
    work->header.state = 0;
}

int TitleSequenceSprite_IsIdle(TitleSequenceSprite *work)
{
    return work->header.state == 4;
}

int TitleSequenceSprite_IsSquashing(TitleSequenceSprite *work)
{
    return work->header.state == 3;
}

void TitleRotatingSprite_StartArc(TitleRotatingSprite *work, int duration, int hold_frames, int start_x,
                                  int start_y, int target_x, int target_y, int unknown_3c, int unknown_40,
                                  int state)
{
    work->sprite.start_x = start_x;
    work->sprite.start_y = start_y;
    work->sprite.target_x = target_x;
    work->sprite.target_y = target_y;
    work->sprite.header.x = start_x << 12;
    work->sprite.header.y = start_y << 12;
    work->sprite.header.elapsed = 0;
    work->sprite.header.duration = duration;
    work->sprite.hold_frames = hold_frames;
    work->sprite.unknown_3c = unknown_3c;
    work->sprite.unknown_40 = unknown_40;
    func_ov006_02070bd4(&work->sprite.acceleration, &work->sprite.velocity, work->sprite.unknown_3c,
                        work->sprite.unknown_40, work->sprite.header.duration);
    work->sprite.header.state = state;
}

void TitleRotatingSprite_StartVerticalMove(TitleRotatingSprite *work, int distance)
{
    work->sprite.start_x = work->sprite.header.x / 4096;
    work->sprite.start_y = work->sprite.header.y / 4096;
    work->sprite.target_x = work->sprite.header.x / 4096;
    work->sprite.target_y = distance + work->sprite.header.y / 4096;
    work->sprite.header.elapsed = 0;
    work->sprite.header.duration = 10;
    work->sprite.header.state = 2;
}

void TitleRotatingSprite_StartSquash(TitleRotatingSprite *work, int duration)
{
    work->sprite.target_x = work->sprite.header.x / 4096;
    work->sprite.target_y = work->sprite.header.y / 4096;
    work->sprite.header.elapsed = 0;
    work->sprite.header.duration = duration;
    work->sprite.header.state = 4;
}

int TitleRotatingSprite_IsIdle(TitleRotatingSprite *work)
{
    return work->sprite.header.state == 8;
}
