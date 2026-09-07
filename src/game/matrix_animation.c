#include <game/matrix_animation.h>
extern const MtxFx44 data_02054f7c;
extern const s16 FX_SinCosTable_[];
extern void func_0202cc58(const void *, void *, u32);
extern void func_0202cd2c(const void *, void *, u32);
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
