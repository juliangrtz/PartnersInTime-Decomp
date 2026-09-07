#include <nitro/os_sync.h>
#include <nitro/snd_command.h>

extern SoundCommand *data_02063058;
extern u32 data_0206305c;
extern SoundCommand *data_02063060;
extern SoundCommand *data_02063064;
extern SoundCommand *data_02063068;
extern int data_0206306c;
extern int data_02063070;
extern int data_02063074;
extern u32 data_02063078;
extern SoundCommand *data_0206307c[9];
extern SoundSharedWork data_020630a0;
extern SoundCommand data_02063320[256];
extern SoundSharedWork *data_02064b80;

extern void func_0203ae6c(u32 cycles);
extern int func_0203d4dc(u32 tag, u32 data, int error);
extern void DC_FlushRange(const void *data, u32 size);

void func_0203ca18(void) {
    int index;
    SoundCommand *command;
    func_0203c3e0();
    data_02063058 = data_02063320;
    for (index = 0; index < 255; index++) {
        data_02063320[index].next = &data_02063320[index + 1];
    }
    data_02063320[255].next = 0;
    data_02063068 = &data_02063320[255];
    data_02063060 = 0;
    data_02063064 = 0;
    data_02063074 = 0;
    data_0206306c = 0;
    data_02063070 = 0;
    data_02063078 = 1;
    data_0206305c = 0;
    data_02064b80 = &data_020630a0;
    func_0203cc10(data_02064b80);
    command = func_0203c848(1);
    if (!command) return;
    command->opcode = 29;
    command->argument[0] = (u32)data_02064b80;
    func_0203c800(command);
    func_0203c640(1);
}

SoundCommand *func_0203c8e4(u32 flags) {
    u32 state = OS_DisableInterrupts();
    SoundCommand *command;
    SoundCommand *tail;
    if (flags & 1) {
        while (data_0206305c == func_0203cc90()) {
            OS_RestoreInterrupts(state);
            func_0203ae6c(100);
            state = OS_DisableInterrupts();
        }
    } else if (data_0206305c == func_0203cc90()) {
        OS_RestoreInterrupts(state);
        return 0;
    }
    command = data_0206307c[data_0206306c];
    data_0206306c++;
    if (data_0206306c > 8) data_0206306c = 0;
    for (tail = command; tail->next; tail = tail->next) {}
    if (data_02063068) data_02063068->next = command;
    else data_02063058 = command;
    data_02063068 = tail;
    data_02063074--;
    data_0206305c++;
    OS_RestoreInterrupts(state);
    return command;
}

SoundCommand *func_0203c848(u32 flags) {
    SoundCommand *command;
    if (!func_0203c30c()) return 0;
    command = func_0203c354();
    if (command) return command;
    if (!(flags & 1)) return 0;
    if (func_0203c47c() > 0) {
        while (func_0203c8e4(0)) {}
        command = func_0203c354();
        if (command) return command;
    } else {
        func_0203c640(1);
    }
    func_0203c3ac();
    do {
        func_0203c8e4(1);
        command = func_0203c354();
    } while (!command);
    return command;
}

void func_0203c800(SoundCommand *command) {
    u32 state = OS_DisableInterrupts();
    if (!data_02063064) {
        data_02063064 = command;
        data_02063060 = command;
    } else {
        data_02063064->next = command;
        data_02063064 = command;
    }
    command->next = 0;
    OS_RestoreInterrupts(state);
}

int func_0203c640(u32 flags) {
    u32 state = OS_DisableInterrupts();
    if (!data_02063060) {
        OS_RestoreInterrupts(state);
        return 1;
    }
    if (data_02063074 >= 8) {
        if (!(flags & 1)) {
            OS_RestoreInterrupts(state);
            return 0;
        }
        do { func_0203c8e4(1); } while (data_02063074 >= 8);
    }
    DC_FlushRange(data_02063320, sizeof(data_02063320));
    if (func_0203d4dc(7, (u32)data_02063060, 0) < 0) {
        if (!(flags & 1)) {
            OS_RestoreInterrupts(state);
            return 0;
        }
        while (func_0203d4dc(7, (u32)data_02063060, 0) < 0) {
            OS_RestoreInterrupts(state);
            func_0203ae6c(100);
            state = OS_DisableInterrupts();
        }
    }
    if (flags & 2) func_0203c3ac();
    data_0206307c[data_02063070] = data_02063060;
    data_02063070++;
    if (data_02063070 > 8) data_02063070 = 0;
    data_02063060 = 0;
    data_02063064 = 0;
    data_02063074++;
    data_02063078++;
    OS_RestoreInterrupts(state);
    return 1;
}

void func_0203c5b0(u32 serial) {
    if (func_0203c51c(serial)) return;
    while (func_0203c8e4(0)) {}
    if (func_0203c51c(serial)) return;
    func_0203c3ac();
    while (!func_0203c51c(serial)) func_0203c8e4(1);
}

