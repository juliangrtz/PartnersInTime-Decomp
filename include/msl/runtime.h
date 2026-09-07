#ifndef MSL_RUNTIME_H
#define MSL_RUNTIME_H

#include <nitro.h>

typedef void (*MslSignalHandler)(int signal);
typedef struct MslFile {
    int handle;
    struct {
        u32 open_mode : 2;
        u32 io_mode : 3;
        u32 buffer_mode : 2;
        u32 file_kind : 3;
        u32 orientation : 2;
        u32 binary_io : 1;
        u32 reserved : 19;
    } mode;
    struct {
        u32 io_state : 3;
        u32 reserved : 29;
    } state;
    u8 unknown0c;
    u8 error;
    u8 unknown0e[10];
    u32 position;
    u8 *buffer;
    u32 buffer_size;
    u8 *cursor;
    u32 buffer_length;
    u32 alignment_mask;
    u32 unknown30;
    u32 buffer_position;
    void *position_callback;
    void *read_callback;
    int (*write_callback)(int handle, void *buffer, u32 *length, void *refcon);
    void *close_callback;
    void *refcon;
} MslFile;

typedef char MslFileSizeCheck[(sizeof(MslFile) == 76) ? 1 : -1];

void MSL_ExitInternal(int status);
void MSL_Exit(int status);
void MSL_Abort(void);
int MSL_FlushAll(void);
int MSL_FlushBuffer(MslFile *file, u32 *written);
void MSL_PrepareBuffer(MslFile *file);
void MSL_ConvertFromNewlines(void *buffer, u32 *length);
int MSL_Flush(MslFile *file);
int MSL_Raise(int signal);

#endif
