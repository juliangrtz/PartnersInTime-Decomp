#include <game/sprite_animation.h>
void GameSpriteAnimation_Scan(GameSpriteAnimation *animation, GameSpriteAnimationTrack *track) {
    const s16 *command = track->commands;
    int duration;
    int end;
    if (!command) return;
    duration = 0;
    end = 0;
    for (;;) {
        switch (*command) {
        case 0:
            track->flags.bits.duration = end / 256;
            return;
        case 1:
            duration = command[1] << 8;
            if (duration < 256) duration = 256;
            command += 2;
            animation->commands = command;
            break;
        case 2: {
            int time;
            if (end < duration + (command[1] << 8)) end = duration + (command[1] << 8);
            time = command[1] << 8;
            if (track->flags.bits.duration) {
                int elapsed = track->time - time;
                if (elapsed >= 0 && elapsed <= duration) {
                    animation->time = elapsed;
                    animation->fraction = 4 * elapsed / (duration / 256);
                    func_02016c0c(animation, track, animation->commands);
                }
            }
            command += 2;
            break;
        }
        case 15: command += command[1] * 2 + 2; break;
        case 16: command += 4; break;
        case 17: command += 4; break;
        default: command += 2; break;
        }
    }
}
