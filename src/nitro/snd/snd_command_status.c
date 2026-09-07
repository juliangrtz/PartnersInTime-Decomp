#include <nitro/os_sync.h>
#include <nitro/snd_command.h>

extern SoundCommand *data_02063058;
extern u32 data_0206305c;
extern SoundCommand *data_02063060;
extern SoundCommand *data_02063068;

extern int func_02039bac(void);
extern void func_0203ae6c(u32 cycles);
extern int func_0203d4dc(u32 tag, u32 data, int error);
extern int func_0203d590(u32 tag, int processor);
extern void func_0203d5b8(u32 tag, void (*callback)(u32, u32, int));

int func_0203c51c(u32 serial) {
    u32 state = OS_DisableInterrupts();
    int completed;
    if (serial > data_0206305c) {
        if ((serial - data_0206305c) < 0x80000000) completed = 0;
        else completed = 1;
    }
    else completed = (data_0206305c - serial) < 0x80000000;
    OS_RestoreInterrupts(state);
    return completed;
}

int func_0203c4dc(void) {
    u32 state = OS_DisableInterrupts();
    int count = 0;
    SoundCommand *command;
    for (command = data_02063058; command; command = command->next) count++;
    OS_RestoreInterrupts(state);
    return count;
}

int func_0203c49c(void) {
    u32 state = OS_DisableInterrupts();
    int count = 0;
    SoundCommand *command;
    for (command = data_02063060; command; command = command->next) count++;
    OS_RestoreInterrupts(state);
    return count;
}

int func_0203c47c(void) {
    int free_count = func_0203c4dc();
    return 256 - free_count - func_0203c49c();
}

void func_0203c44c(u32 tag, u32 value, int error) {
    u32 state = OS_DisableInterrupts();
    func_0203cb30(value);
    OS_RestoreInterrupts(state);
}

void func_0203c3e0(void) {
    func_0203d5b8(7, func_0203c44c);
    if (!func_0203c30c()) return;
    while (!func_0203d590(7, 1)) func_0203ae6c(100);
}

void func_0203c3ac(void) { while (func_0203d4dc(7, 0, 0) < 0) {} }

SoundCommand *func_0203c354(void) {
    u32 state = OS_DisableInterrupts();
    SoundCommand *command = data_02063058;
    if (!command) {
        OS_RestoreInterrupts(state);
        return 0;
    }
    data_02063058 = command->next;
    if (!data_02063058) data_02063068 = 0;
    OS_RestoreInterrupts(state);
    return command;
}

int func_0203c30c(void) {
    u32 state;
    u32 available;
    if (!func_02039bac()) return 1;
    state = OS_DisableInterrupts();
    *(vu32 *)0x04FFF200 = 16;
    available = *(vu32 *)0x04FFF200;
    OS_RestoreInterrupts(state);
    return available != 0;
}
