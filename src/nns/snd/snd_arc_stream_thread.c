/*
 * Stream thread (ARM9 resident, 0x020322E0-0x02032350).
 *
 * The worker thread that refills the stream buffers.
 */

#include <nns/snd_arc_stream.h>

extern void func_02032350(NNSiSndArcStrmCommand *command);

void NNSi_SndArcStrmThreadMain(void *argument)
{
    NNSiSndArcStrmThread *thread = argument;
sleep:
    OS_SleepThread(&thread->queue);
    for (;;) {
        NNSiSndArcStrmCommand *command;
        func_02039ecc(&thread->mutex);
        command = NNSi_SndArcStrmPopCommand(&thread->commands);
        if (!command) {
            func_02039e5c(&thread->mutex);
            goto sleep;
        }
        func_02032350(command);
        NNSi_SndArcStrmFreeCommand(command);
        func_02039e5c(&thread->mutex);
    }
}
