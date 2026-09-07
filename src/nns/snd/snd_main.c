#include <nns/snd.h>
#include <nitro/pm.h>

extern int data_02060ea0;
extern s8 data_02060e98;
extern int data_02060e9c;
extern PmSleepCallback data_02060ea4;
extern PmSleepCallback data_02060eb0;

extern void NNSi_SndCapturePostSleep(void);
extern void NNSi_SndCapturePreSleep(void);
extern void NNSi_SndCaptureMain(void);
extern void NNSi_SndCaptureInit(void);
extern void NNSi_SndArcStrmMain(void);
extern void func_0203c2c0(void);
extern void func_0203c0b0(u32 channels, u32 captures, u32 alarms, u32 flags);
extern u32 func_0203c570(void);
extern void func_0203c5b0(u32 command);
extern int func_0203c640(int flags);
extern void *func_0203c8e4(int flags);
extern void PM_PrependPreSleepCallback(PmSleepCallback *callback);
extern void PM_AppendPostSleepCallback(PmSleepCallback *callback);

void NNSi_SndPreSleep(void *argument);
void NNSi_SndPostSleep(void *argument);

void NNS_SndInit(void)
{
    if (data_02060ea0) {
        return;
    }
    data_02060ea0 = 1;
    func_0203c2c0();
    data_02060ea4.callback = NNSi_SndPreSleep;
    data_02060ea4.argument = 0;
    data_02060eb0.callback = NNSi_SndPostSleep;
    data_02060eb0.argument = 0;
    PM_PrependPreSleepCallback(&data_02060ea4);
    PM_AppendPostSleepCallback(&data_02060eb0);
    NNSi_SndInitResource();
    NNSi_SndCaptureInit();
    NNSi_SndPlayerInit();
    data_02060e98 = -1;
    data_02060e9c = 1;
}

void NNS_SndMain(void)
{
    while (func_0203c8e4(0)) {
    }
    NNSi_SndPlayerMain();
    NNSi_SndCaptureMain();
    NNSi_SndArcStrmMain();
    func_0203c640(0);
}

void NNSi_SndPreSleep(void *argument)
{
    u32 command;
    NNSi_SndCapturePreSleep();
    func_0203c0b0(0, 0, 0, 0);
    command = func_0203c570();
    func_0203c640(1);
    func_0203c5b0(command);
}

void NNSi_SndPostSleep(void *argument)
{
    NNSi_SndCapturePostSleep();
}
