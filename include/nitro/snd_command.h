#ifndef NITRO_SND_COMMAND_H
#define NITRO_SND_COMMAND_H

#include <nitro.h>

typedef struct SoundCommand {
    struct SoundCommand *next;
    u32 opcode;
    u32 argument[4];
} SoundCommand;

typedef void (*SoundCallback)(void *argument);
typedef struct SoundAlarmCallback {
    SoundCallback callback;
    void *argument;
    u8 generation;
    u8 padding09[3];
} SoundAlarmCallback;

typedef struct SoundSharedTrack {
    s16 variables[16];
    u32 flags;
} SoundSharedTrack;

typedef struct SoundSharedWork {
    u32 completed_serial;
    u32 status;
    u16 channel_status;
    u16 capture_status;
    u8 unknown0c[20];
    SoundSharedTrack tracks[16];
    s16 variables[16];
} SoundSharedWork;

typedef char SoundCommandSizeCheck[(sizeof(SoundCommand) == 24) ? 1 : -1];
typedef char SoundAlarmCallbackSizeCheck[(sizeof(SoundAlarmCallback) == 12) ? 1 : -1];
typedef char SoundSharedWorkSizeCheck[(sizeof(SoundSharedWork) == 640) ? 1 : -1];

void func_0203bd6c(u32 opcode, u32 arg0, u32 arg1, u32 arg2, u32 arg3);
void func_0203bdf0(u32 arg0, u32 arg1, u32 arg2, u32 arg3, u32 width);
void func_0203be24(u32 arg0, u32 arg1, u32 arg2, u32 arg3);
void func_0203c298(void);
void func_0203c2ac(void);
int func_0203c30c(void);
SoundCommand *func_0203c354(void);
void func_0203c3ac(void);
void func_0203c3e0(void);
void func_0203c44c(u32 tag, u32 value, int error);
int func_0203c47c(void);
int func_0203c49c(void);
int func_0203c4dc(void);
int func_0203c51c(u32 serial);
int func_0203c640(u32 flags);
void func_0203c800(SoundCommand *command);
SoundCommand *func_0203c848(u32 flags);
SoundCommand *func_0203c8e4(u32 flags);
void func_0203ca18(void);
void func_0203cb30(s32 value);
u8 func_0203cb90(u32 index, SoundCallback callback, void *argument);
void func_0203cbc0(u32 index);
void func_0203cbe0(void);
void func_0203cc10(SoundSharedWork *work);
u32 func_0203cc90(void);

#endif
