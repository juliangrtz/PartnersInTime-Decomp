#include <nitro/snd_command.h>

extern SoundAlarmCallback data_02064b20[8];
extern SoundSharedWork *data_02064b80;
extern void DC_FlushRange(const void *data, u32 size);
extern void DC_InvalidateRange(const void *data, u32 size);

u32 func_0203ccc4(void) {
    DC_InvalidateRange(data_02064b80, sizeof(SoundSharedWork));
    return data_02064b80->status;
}

u32 func_0203cc90(void) {
    DC_InvalidateRange(data_02064b80, sizeof(SoundSharedWork));
    return data_02064b80->completed_serial;
}

void func_0203cc10(SoundSharedWork *work) {
    int track;
    int variable;
    work->status = 0;
    work->channel_status = 0;
    work->capture_status = 0;
    work->completed_serial = 0;
    for (track = 0; track < 16; track++) {
        work->tracks[track].flags = 0;
        for (variable = 0; variable < 16; variable++) {
            work->tracks[track].variables[variable] = -1;
        }
    }
    for (variable = 0; variable < 16; variable++) work->variables[variable] = -1;
    DC_FlushRange(work, sizeof(*work));
}

void func_0203cbe0(void) {
    int index;
    for (index = 0; index < 8; index++) {
        data_02064b20[index].callback = 0;
        data_02064b20[index].argument = 0;
        data_02064b20[index].generation = 0;
    }
}

void func_0203cbc0(u32 index) {
    SoundAlarmCallback *alarm = &data_02064b20[index];
    alarm->generation++;
}

u8 func_0203cb90(u32 index, SoundCallback callback, void *argument) {
    SoundAlarmCallback *alarm = &data_02064b20[index];
    alarm->callback = callback;
    alarm->argument = argument;
    alarm->generation++;
    return alarm->generation;
}

void func_0203cb30(s32 value) {
    SoundAlarmCallback *alarm = &data_02064b20[(u8)value];
    u8 generation = (value >> 8) & 0xFF;
    if (generation != alarm->generation) return;
    if (!alarm->callback) return;
    alarm->callback(alarm->argument);
}
