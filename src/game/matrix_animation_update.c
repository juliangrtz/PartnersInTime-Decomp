#include <game/matrix_animation.h>
extern const s16 *data_0205a8f0;
extern int data_0205a8f4;
extern void func_0202cbd4(void *, int, u32);

void GameMatrixAnimation_Update(GameMatrixAnimation *animation) {
    GameMatrixAnimationTrack *track;
    for (track = animation->active; track; track = track->next) {
        data_0205a8f0 = track->commands;
        if (track->commands && track->draw) {
            struct { s16 start, index; } command = {0, 0};
            animation->resource = track;
            data_0205a8f4 = track->previous_time - track->current_time;
            if (track->flags.bits.separate_sequences) {
                for (; command.index < track->commands[0]; ++command.index) {
                    int duration = track->commands[track->commands[command.index + 1]];
                    int time = track->current_time;
                    if (!track->flags.bits.loop) {
                        if (time < 0) time = 0;
                        if ((duration << 12) < time) time = duration << 12;
                    } else if (duration) {
                        int length = duration << 12;
                        time %= length;
                        if (time < 0) time += length;
                    }
                    animation->matrix = animation->matrices + 1;
                    animation->evaluate = GameAnimation_EvaluateLinear;
                    GameMatrixAnimation_Execute(animation, (s16 *)&command, time);
                }
            } else {
                if (!track->flags.bits.loop) {
                    if (track->current_time < 0) track->current_time = 0;
                    if ((track->duration << 12) < track->current_time)
                        track->current_time = track->duration << 12;
                } else if (track->duration) {
                    while (track->current_time < 0) track->current_time += track->duration << 12;
                    while ((track->duration << 12) < track->current_time) track->current_time -= track->duration << 12;
                }
                animation->matrix = animation->matrices + 1;
                animation->evaluate = GameAnimation_EvaluateLinear;
                func_0202cbd4(animation->parameters, 0, 64);
                GameMatrixAnimation_Execute(animation, (s16 *)&command, track->current_time);
            }
            track->previous_time = track->current_time;
            if (!track->flags.bits.paused) {
                track->flags.bits.finished = 0;
                if (track->flags.bits.loop) {
                    track->current_time += track->speed;
                } else if (track->speed > 0 && (track->duration << 12) <= track->current_time) {
                    track->flags.bits.finished = 1;
                    if (!track->flags.bits.retain) track->commands = 0;
                } else if (track->speed < 0 && track->current_time <= 0) {
                    track->flags.bits.finished = 1;
                    if (!track->flags.bits.retain) track->commands = 0;
                } else {
                    track->current_time += track->speed;
                }
            }
        }
        if ((!track->commands || !track->draw) && track->finish) track->finish(track);
    }
    {
        GameMatrixAnimationTrack *current = animation->active;
        GameMatrixAnimationTrack *previous;
        previous = 0;
        while (current) {
            if (current->commands && current->draw) {
                previous = current;
                current = current->next;
            } else {
                animation->free_tail->next = current;
                animation->free_tail = current;
                if (previous) previous->next = current->next;
                else animation->active = current->next;
                current = current->next;
            }
        }
    }
}
