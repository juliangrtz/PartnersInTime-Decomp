#include <game/matrix_animation.h>
extern int data_0205a8f4;
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
