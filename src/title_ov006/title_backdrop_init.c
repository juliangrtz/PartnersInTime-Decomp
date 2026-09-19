#include "title_sequence_internal.h"
extern const TitleBackdropPoint data_ov006_0207b1d0[], data_ov006_0207b218[];
extern const TitleBackdropPoint data_ov006_0207b254[], data_ov006_0207b27c[];
void func_ov006_02073e6c(void *, int, int);
void func_ov006_02073dc0(void *element);

void TitleBackdrop_InitAll(TitleBackdrop *work)
{
    int depth = 64;
    int group;
    for (group = 0; group < 4; ++group) {
        const TitleBackdropPoint *points;
        int count, animation, i;
        s16 velocity;
        switch (group) {
        case 0: points = data_ov006_0207b1d0; count = 4; velocity = -2457; animation = 10; break;
        case 1: points = data_ov006_0207b218; count = 5; velocity = -2048; animation = 10; break;
        case 2: points = data_ov006_0207b254; count = 8; velocity = -1228; animation = 11; break;
        case 3: points = data_ov006_0207b27c; count = 8; velocity = -819; animation = 11; break;
        }
        ++points;
        for (i = 0; i < count; ++i, ++points) {
            TitleModel_Load(work, 0xc2000033, 2, 0, 1);
            func_ov006_02073e6c(work, animation, 1);
            work->model.header.x = points->x << 12;
            work->model.header.y = (points->y + 244) << 12;
            work->model.header.depth = depth;
            work->velocity_x = velocity;
            work->model.header.update = func_ov006_02073dc0;
            work->group = group;
            work->model.sub_model->flag_bits.unknown_00_01 = 3;
            ++work;
        }
        depth += 64;
    }
}
void TitleBackdrop_Release(TitleBackdrop *work)
{
    TitleModel_Release(work);
}
void TitleBackdrop_OnSkip(TitleBackdrop *work)
{
}
