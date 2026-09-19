/*
 * Sound command channel (ARM9 resident, 0x0203BD6C-0x0203C570).
 *
 * The ARM9 side of the sound command channel: the command emitters, the mutex
 * that serializes them, and the PXI send and completion polling. Commands are
 * batched into the queue and flushed together.
 */

#include <nitro/os_sync.h>
#include <nitro/snd_command.h>

extern SoundCommand *data_02063058;
extern u32 data_0206305c;
extern SoundCommand *data_02063060;
extern SoundCommand *data_02063068;

extern int func_02039bac(void);
extern void func_0203ae6c(u32 cycles);
extern int PXI_SendWordByFifo(u32 tag, u32 data, int error);
extern int PXI_IsCallbackReady(u32 tag, int processor);
extern void PXI_SetFifoRecvCallback(u32 tag, void (*callback)(u32, u32, int));

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
    PXI_SetFifoRecvCallback(7, func_0203c44c);
    if (!func_0203c30c()) return;
    while (!PXI_IsCallbackReady(7, 1)) func_0203ae6c(100);
}

void func_0203c3ac(void) { while (PXI_SendWordByFifo(7, 0, 0) < 0) {} }

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

extern int data_02063040;
extern OsMutex data_02063044;
extern void func_02039e5c(OsMutex *mutex);
extern void func_02039ecc(OsMutex *mutex);
extern void func_02039f58(OsMutex *mutex);

void func_0203c2c0(void) {
    if (data_02063040) return;
    data_02063040 = 1;
    func_02039f58(&data_02063044);
    func_0203ca18();
    func_0203cbe0();
}
void func_0203c2ac(void) { func_02039ecc(&data_02063044); }
void func_0203c298(void) { func_02039e5c(&data_02063044); }

void func_0203c26c(u32 arg0) { func_0203bd6c(1, arg0, 0, 0, 0); }
void func_0203c238(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    func_0203bd6c(2, arg0, arg1, arg2, arg3);
}
void func_0203c20c(u32 arg0) { func_0203bd6c(3, arg0, 0, 0, 0); }
void func_0203c1f4(u32 arg0, u32 arg1) { func_0203be24(arg0, 6, arg1, 2); }
void func_0203c1dc(u32 arg0, u32 arg1) { func_0203be24(arg0, 4, arg1, 1); }
void func_0203c1b0(u32 arg0, u32 arg1) { func_0203bd6c(11, arg0, arg1, 0, 0); }
void func_0203c188(u32 arg0, u32 arg1, u32 arg2) {
    func_0203bdf0(arg0, arg1, 10, arg2, 2);
}
void func_0203c154(u32 arg0, u32 arg1, u32 arg2) {
    func_0203bd6c(9, arg0, arg1, arg2, 0);
}
void func_0203c120(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    func_0203bd6c(12, arg0, arg1, arg2, arg3);
}
void func_0203c0b0(u32 arg0, u32 arg1, u32 alarm_mask, u32 arg3) {
    int index;
    u32 remaining = alarm_mask;
    for (index = 0; index < 8 && remaining; index++, remaining >>= 1) {
        if (remaining & 1) func_0203cbc0(index);
    }
    func_0203bd6c(13, arg0, arg1, alarm_mask, arg3);
}
void func_0203c064(u32 flag0, u32 flag1, u32 arg0, u32 arg1,
                   u32 flag2, u32 flag3, u32 flag4) {
    func_0203bd6c(17, arg0, arg1,
        (flag0 << 31) | (flag1 << 30) | (flag2 << 29) | (flag3 << 28) | (flag4 << 27), 0);
}
void func_0203c020(u32 index, u32 arg1, u32 arg2, SoundCallback callback, void *argument) {
    u32 generation = func_0203cb90(index, callback, argument);
    func_0203bd6c(18, index, arg1, arg2, generation);
}
void func_0203bff4(u32 arg0, u32 arg1) { func_0203bd6c(26, arg0, arg1, 0, 0); }
void func_0203bfc8(u32 arg0, u32 arg1) { func_0203bd6c(27, arg0, arg1, 0, 0); }
void func_0203bf94(u32 arg0, u32 arg1, u32 arg2) { func_0203bd6c(20, arg0, arg1, arg2, 0); }
void func_0203bf68(u32 arg0, u32 arg1) { func_0203bd6c(21, arg0, arg1, 0, 0); }
void func_0203bf10(u32 arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4,
                   u32 arg5, u32 arg6, u32 arg7, u32 arg8, u32 arg9) {
    func_0203bd6c(14, arg0 | (arg8 << 16), arg2,
        arg5 | ((arg6 << 24) | (arg7 << 22)),
        arg4 | ((arg3 << 26) | (arg1 << 24) | (arg9 << 16)));
}
void func_0203bee4(u32 arg0, u32 arg1) { func_0203bd6c(30, arg0, arg1, 0, 0); }
void func_0203beb8(u32 arg0, u32 arg1) { func_0203bd6c(31, arg0, arg1, 0, 0); }
void func_0203be8c(u32 arg0, u32 arg1) { func_0203bd6c(32, arg0, arg1, 0, 0); }
void func_0203be58(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    func_0203bd6c(25, arg0, arg1, arg2, arg3);
}
void func_0203be24(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    func_0203bd6c(6, arg0, arg1, arg2, arg3);
}
void func_0203bdf0(u32 arg0, u32 arg1, u32 arg2, u32 arg3, u32 width) {
    func_0203bd6c(7, arg0 | (width << 24), arg1, arg2, arg3);
}
void SND_SetSurroundDecay(u32 arg0) { func_0203bd6c(22, arg0, 0, 0, 0); }
void func_0203bd6c(u32 opcode, u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    SoundCommand *command = func_0203c848(1);
    if (!command) return;
    command->opcode = opcode;
    command->argument[0] = arg0;
    command->argument[1] = arg1;
    command->argument[2] = arg2;
    command->argument[3] = arg3;
    func_0203c800(command);
}
