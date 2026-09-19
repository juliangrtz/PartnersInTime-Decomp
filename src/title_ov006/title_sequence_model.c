#include "title_sequence_internal.h"
void func_ov006_02073e6c(void *, int, int);

void TitleSequenceModel_Update(void *element)
{
    TitleSequenceModelElement *work = element;
    switch (work->header.state) {
    case TITLE_SEQUENCE_MODEL_IDLE:
        break;
    case TITLE_SEQUENCE_MODEL_DELAY:
        ++work->header.elapsed;
        if (work->header.elapsed >= 8) {
            work->header.elapsed = 0;
            work->header.state = TITLE_SEQUENCE_MODEL_MOVING;
        }
        break;
    case TITLE_SEQUENCE_MODEL_MOVING:
        ++work->header.elapsed;
        if (work->header.elapsed < 24) {
            work->header.x = 128 << 12;
            work->header.y = (500 + (-76 * work->header.elapsed) / 24) << 12;
        } else {
            work->header.x = 128 << 12;
            work->header.y = 424 << 12;
            work->header.state = TITLE_SEQUENCE_MODEL_IDLE;
        }
        break;
    }
}

void TitleSequenceModel_Init(TitleSequenceModelElement *work)
{
    TitleModel_Load(work, 0xC2000033, 2, 0, 1);
    func_ov006_02073e6c(work, 15, 1);
    work->header.x = 128 << 12;
    work->header.y = 500 << 12;
    work->header.depth = 16;
    work->header.state = TITLE_SEQUENCE_MODEL_IDLE;
    work->header.update = TitleSequenceModel_Update;
}

void TitleSequenceModel_StartEntry(TitleSequenceModelElement *work)
{
    work->header.elapsed = 0;
    work->header.state = TITLE_SEQUENCE_MODEL_DELAY;
}

void TitleSequenceModel_FinishEntry(TitleSequenceModelElement *work)
{
    work->header.elapsed = 0;
    work->header.x = 128 << 12;
    work->header.y = 424 << 12;
    work->header.state = TITLE_SEQUENCE_MODEL_IDLE;
}
