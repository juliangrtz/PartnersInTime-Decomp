#include <game/matrix_animation.h>
extern const MtxFx44 data_02054f7c;
extern void func_0202cbd4(void *, int, u32);
extern void func_0202cc58(const void *, void *, u32);
extern void func_0202cd2c(const void *, void *, u32);

GameMatrixAnimation *GameMatrixAnimation_Initialize(GameMatrixAnimation *animation, void *storage, int tracks, int matrices) {
    GameMatrixAnimationTrack *track;
    int i;
    func_0202cbd4(animation, 0, 100);
    func_0202cbd4(storage, 0, tracks * 56 + (matrices << 6));
    animation->storage = storage;
    animation->matrices = (MtxFx44 *)(animation->storage + tracks);
    if (&data_02054f7c < animation->matrices)
        func_0202cd2c(&data_02054f7c, animation->matrices, 64);
    else
        func_0202cc58(&data_02054f7c, animation->matrices, 64);
    track = animation->storage;
    animation->free_head = animation->storage;
    for (i = 0; i < tracks - 1; ++i, ++track) track->next = track + 1;
    track->next = 0;
    animation->free_tail = track;
    animation->active = 0;
    return animation;
}
void GameMatrixAnimation_Finalize(GameMatrixAnimation *animation) {}
GameMatrixAnimationTrack *GameMatrixAnimation_Allocate(GameMatrixAnimation *animation) {
    GameMatrixAnimationTrack *track = animation->free_head;
    if (track == animation->free_tail) return 0;
    animation->free_head = track->next;
    return track;
}
GameMatrixAnimationTrack *GameMatrixAnimation_Start(GameMatrixAnimation *animation, const s16 *commands,
    void (*draw)(int, MtxFx44 *, GameMatrixAnimationTrack *), int speed) {
    GameMatrixAnimationTrack *track;
    if (!commands) return 0;
    if (!draw) return 0;
    track = GameMatrixAnimation_Allocate(animation);
    if (track) {
        track->next = animation->active;
        animation->active = track;
        track->commands = commands;
        track->duration = commands[commands[1]];
        track->current_time = speed < 0 ? track->duration : 0;
        track->previous_time = track->current_time - speed;
        track->speed = speed;
        track->flags.bits.loop = 0;
        track->flags.bits.paused = 0;
        track->flags.bits.separate_sequences = 0;
        track->flags.bits.retain = 0;
        track->flags.bits.finished = 0;
        track->draw = draw;
        track->context = 0;
        track->parameters[0] = 0;
        track->parameters[1] = 0;
        track->parameters[2] = 0;
        track->finish = GameMatrixAnimation_ReleaseOwner;
        track->owner = 0;
    }
    return track;
}

extern const s16 *data_0205a8f0;
extern int data_0205a8f4;

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

extern const s16 FX_SinCosTable_[];
extern void MTX_ApplyRotZ44(int, int, s32 *, s32 *);

void GameMatrixAnimation_Execute(GameMatrixAnimation *animation, const s16 *command, int time) {
    s32 *matrix;
    const s16 *sequence;
    time -= command[0] << 12;
    if (time < 0) return;
    sequence = animation->resource->commands;
    sequence += sequence[command[1] + 1];
    if ((*sequence << 12) < time) return;
    matrix = (s32 *)animation->matrix;
    command = sequence + 1;
    if ((const void *)&data_02054f7c < (void *)matrix)
        func_0202cd2c(&data_02054f7c, matrix, 64);
    else
        func_0202cc58(&data_02054f7c, matrix, 64);
    for (;;) {
        switch (*command) {
        case 0: return;
        case 1:
            animation->time = time;
            MTX_Concat44((MtxFx44 *)matrix, (MtxFx44 *)(matrix - 16), (MtxFx44 *)(matrix + 16));
            animation->resource->draw(command[1], (MtxFx44 *)(matrix + 16), animation->resource);
            command += 2;
            break;
        case 2:
            if (command[2] >= 0 && command[2] < animation->resource->commands[0]) {
                MTX_Concat44((MtxFx44 *)matrix, (MtxFx44 *)(matrix - 16), (MtxFx44 *)(matrix + 16));
                animation->matrix = (MtxFx44 *)(matrix + 32);
                GameMatrixAnimation_Execute(animation, command + 1, time);
                animation->matrix = (MtxFx44 *)(matrix - 32);
            }
            command += 3;
            break;
        case 3: {
            int scale = animation->evaluate(command, time);
            matrix[0] = matrix[0] * scale / 4096;
            matrix[4] = matrix[4] * scale / 4096;
            matrix[8] = matrix[8] * scale / 4096;
            matrix[12] = matrix[12] * scale / 4096;
            command += command[1];
            break;
        }
        case 4: {
            int scale = animation->evaluate(command, time);
            matrix[1] = matrix[1] * scale / 4096;
            matrix[5] = matrix[5] * scale / 4096;
            matrix[9] = matrix[9] * scale / 4096;
            matrix[13] = matrix[13] * scale / 4096;
            command += command[1];
            break;
        }
        case 5: {
            int scale = animation->evaluate(command, time);
            matrix[2] = matrix[2] * scale / 4096;
            matrix[6] = matrix[6] * scale / 4096;
            matrix[10] = matrix[10] * scale / 4096;
            matrix[14] = matrix[14] * scale / 4096;
            command += command[1];
            break;
        }
        case 6:
            matrix[12] += animation->evaluate(command, time) << 8;
            command += command[1];
            break;
        case 7:
            matrix[13] += animation->evaluate(command, time) << 8;
            command += command[1];
            break;
        case 8:
            matrix[14] += animation->evaluate(command, time) << 8;
            command += command[1];
            break;
        case 9: {
            int angle = (int)((animation->evaluate(command, time) << 4) & 0xffffu) >> 4;
            MTX_ApplyRotZ44(FX_SinCosTable_[2 * angle + 1], FX_SinCosTable_[2 * angle], matrix + 1, matrix + 2);
            command += command[1];
            break;
        }
        case 10: {
            int angle = (int)((animation->evaluate(command, time) << 4) & 0xffffu) >> 4;
            MTX_ApplyRotZ44(FX_SinCosTable_[2 * angle + 1], FX_SinCosTable_[2 * angle], matrix + 2, matrix + 0);
            command += command[1];
            break;
        }
        case 11: {
            int angle = (int)((animation->evaluate(command, time) << 4) & 0xffffu) >> 4;
            MTX_ApplyRotZ44(FX_SinCosTable_[2 * angle + 1], FX_SinCosTable_[2 * angle], matrix + 0, matrix + 1);
            command += command[1];
            break;
        }
        case 12:
            switch (command[1]) {
            case 0: animation->evaluate = GameAnimation_EvaluateLinear; break;
            case 1: animation->evaluate = GameAnimation_EvaluateStep; break;
            case 2: animation->evaluate = GameAnimation_EvaluateSmooth; break;
            case 3: animation->evaluate = GameAnimation_EvaluateLoopSmooth; break;
            case 4: animation->evaluate = GameAnimation_EvaluateChanged; break;
            }
            command += 2;
            break;
        case 13:
        case 14:
        case 15:
        default: {
            int property = *command;
            ((s32 *)animation + property)[-8] = animation->evaluate(command, time);
            command += command[1];
            break;
        }
        }
    }
}

int GameAnimation_EvaluateStep(const s16 *command, int time);

int GameAnimation_EvaluateLinear(const s16 *command, int time) {
    const s16 *end = command + command[1];
    int value;
    int duration;
    time /= 256;
    command += 2;
    do {
        value = *command++;
        if (end <= command) return value * 16;
        duration = *command++;
        time -= duration * 16;
    } while (time >= 0);
    if (!duration) return *command * 16;
    { int next = *command; return time * (next - value) / duration + next * 16; }
}
int GameAnimation_EvaluateStep(const s16 *command, int time) {
    const s16 *end = command + command[1];
    int value;
    time /= 4096;
    command += 2;
    do {
        int duration;
        value = *command++;
        if (end <= command) return value * 16;
        duration = *command++;
        time -= duration;
    } while (time >= 0);
    return value * 16;
}
int GameAnimation_EvaluateSmooth(const s16 *command, int time) {
    const s16 *start = command;
    const s16 *end = command + command[1] - 1;
    int value;
    int next;
    int total;
    int weight;
    int duration;
    time /= 256;
    do {
        command += 2;
        if (end <= command) return *command * 16;
        duration = command[1] * 16;
        time -= duration;
    } while (time >= 0);
    next = command[2] * 16;
    time += duration;
    value = *command * 16;
    total = value * duration + time * (next - value);
    weight = duration;
    if (command + 3 < end) {
        int following_duration = command[3];
        int following = command[4];
        if (following_duration)
            total += time * ((next / 16 - following) * (duration - time)) / following_duration;
        total += next * time;
        weight += time;
    }
    if (start < command - 2) {
        int following_duration = command[-1];
        int following = command[-2];
        if (following_duration)
            total += time * ((value / 16 - following) * (duration - time)) / following_duration;
        total += value * (duration - time);
        weight += duration - time;
    }
    return total / weight;
}
int GameAnimation_EvaluateLoopSmooth(const s16 *command, int time) {
    const s16 *end;
    const s16 *start = command;
    int value;
    int next;
    int total;
    int weight;
    int duration;
    end = command + command[1] - 1;
    time /= 256;
    do {
        command += 2;
        if (end <= command) return *command * 16;
        duration = command[1] * 16;
        time -= duration;
    } while (time >= 0);
    next = command[2] * 16;
    time += duration;
    value = *command * 16;
    total = value * duration + time * (next - value);
    weight = duration;
    if (command + 3 < end || start[2] == *end) {
        int following_duration;
        int following;
        if (command + 3 < end) { following_duration = command[3]; following = command[4]; }
        else { following_duration = start[3]; following = start[4]; }
        if (following_duration)
            total += time * ((next / 16 - following) * (duration - time)) / following_duration;
        total += next * time;
        weight += time;
    }
    if (start < command - 2 || start[2] == *end) {
        int following_duration;
        int following;
        if (start < command - 2) { following_duration = command[-1]; following = command[-2]; }
        else { following_duration = end[-1]; following = end[-2]; }
        if (following_duration)
            total += time * ((value / 16 - following) * (duration - time)) / following_duration;
        total += value * (duration - time);
        weight += duration - time;
    }
    return total / weight;
}
int GameAnimation_EvaluateChanged(const s16 *command, int time) {
    int value = GameAnimation_EvaluateStep(command, time);
    if (time + data_0205a8f4 < 0) return value;
    if (value == GameAnimation_EvaluateStep(command, time + data_0205a8f4)) value = -1;
    return value;
}

void GameMatrixAnimation_ReleaseOwner(GameMatrixAnimationTrack *track) {
    if (track->owner && *track->owner == track) *track->owner = 0;
}
