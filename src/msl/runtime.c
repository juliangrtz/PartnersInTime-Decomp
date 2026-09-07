#include <msl/runtime.h>
#include <nitro/os_sync.h>

extern u32 data_02065910[9];
extern u32 data_02065934[9];
extern OsMutex data_02065958[9];
extern OsMutex data_020659e4;
extern void (*data_02065800)(void);
extern void (*data_02065804)(void);
extern int data_02065808;
extern int data_0206580c;
extern void (*data_02065810[64])(void);
extern MslSignalHandler data_02065d0c[7];
extern MslFile data_02059db4[3];
extern int func_02039d98(OsMutex *mutex);
extern void func_02039ecc(OsMutex *mutex);
extern void func_02039e5c(OsMutex *mutex);
extern void MSL_DestroyGlobalChain(void);
extern void func_02046dd4(void);

static inline void MSL_EnterCriticalRegion(int region, OsMutex *mutex)
{
    if (!func_02039d98(mutex)) {
        data_02065910[region] = OSi_ThreadInfo.current->id;
        data_02065934[region] = 1;
    } else {
        u32 owner = data_02065910[region];
        u32 thread_id = OSi_ThreadInfo.current->id;
        if (owner == thread_id) {
            ++data_02065934[region];
        } else {
            func_02039ecc(mutex);
            data_02065910[region] = OSi_ThreadInfo.current->id;
            data_02065934[region] = 1;
        }
    }
}

static inline void MSL_LeaveCriticalRegion(int region, OsMutex *mutex)
{
    if (!--data_02065934[region]) {
        func_02039e5c(mutex);
    }
}

int MSL_Raise(int signal)
{
    MslSignalHandler handler;
    if (signal < 1 || signal > 7) {
        return -1;
    }
    MSL_EnterCriticalRegion(7, &data_020659e4);
    handler = data_02065d0c[signal - 1];
    if (handler != (MslSignalHandler)1) {
        data_02065d0c[signal - 1] = 0;
    }
    MSL_LeaveCriticalRegion(7, &data_020659e4);
    if (handler == (MslSignalHandler)1 || (!handler && signal == 1)) {
        return 0;
    }
    if (!handler) {
        MSL_Exit(0);
    }
    handler(signal);
    return 0;
}

int MSL_Flush(MslFile *file)
{
    if (!file) {
        return MSL_FlushAll();
    }
    if (file->error || !file->mode.file_kind) {
        return -1;
    }
    if (file->mode.io_mode == 1) {
        return 0;
    }
    if (file->state.io_state >= 3) {
        file->state.io_state = 2;
    }
    if (file->state.io_state == 2) {
        file->buffer_length = 0;
    }
    if (file->state.io_state != 1) {
        file->state.io_state = 0;
        return 0;
    }
    if (MSL_FlushBuffer(file, 0)) {
        file->error = 1;
        file->buffer_length = 0;
        return -1;
    } else {
        file->state.io_state = 0;
        file->position = 0;
        file->buffer_length = 0;
        return 0;
    }
}

void MSL_ConvertFromNewlines(void *buffer, u32 *length)
{
}

void MSL_PrepareBuffer(MslFile *file)
{
    file->cursor = file->buffer;
    file->buffer_length = file->buffer_size;
    file->buffer_length -= file->position & file->alignment_mask;
    file->buffer_position = file->position;
}

int MSL_FlushBuffer(MslFile *file, u32 *written)
{
    u32 length = file->cursor - file->buffer;
    if (length) {
        int result;
        file->buffer_length = length;
        if (!file->mode.binary_io) {
            MSL_ConvertFromNewlines(file->buffer, &file->buffer_length);
        }
        result = file->write_callback(file->handle, file->buffer, &file->buffer_length, file->refcon);
        if (written) {
            *written = file->buffer_length;
        }
        if (result) {
            return result;
        }
        file->position += file->buffer_length;
    }
    MSL_PrepareBuffer(file);
    return 0;
}

int MSL_FlushAll(void)
{
    MslFile *file = &data_02059db4[0];
    int result = 0;
    int index = 1;
    do {
        if (file->mode.file_kind && MSL_Flush(file)) {
            result = -1;
        }
        file = index < 3 ? &data_02059db4[index++] : 0;
    } while (file);
    return result;
}

void MSL_Abort(void)
{
    MSL_Raise(1);
    data_0206580c = 1;
    MSL_Exit(1);
}

void MSL_Exit(int status)
{
    if (!data_0206580c) {
        MSL_DestroyGlobalChain();
        if (data_02065804) {
            data_02065804();
            data_02065804 = 0;
        }
    }
    MSL_ExitInternal(status);
}

void MSL_ExitInternal(int status)
{
    MSL_EnterCriticalRegion(0, &data_02065958[0]);
    while (data_02065808 > 0) {
        data_02065810[--data_02065808]();
    }
    MSL_LeaveCriticalRegion(0, &data_02065958[0]);
    if (data_02065800) {
        data_02065800();
        data_02065800 = 0;
    }
    MSL_Flush(0);
    func_02046dd4();
}
