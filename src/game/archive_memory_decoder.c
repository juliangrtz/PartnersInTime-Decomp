#include <game/archive_io.h>
#include <game/task.h>

extern u32 data_02059c18[];
extern s16 data_02060b2c[];
extern void GameHeap_Delete(void *allocation);
extern void func_0202bf10(u8 **source, u8 **destination);
extern void func_0202bfbc(u8 **source, u8 **destination);

ArchiveMemoryDecoder *ArchiveMemoryDecoder_Init(ArchiveMemoryDecoder *decoder, u32 priority, u32 unused, void *argument)
{
    GameTask_Init((GameTask *)decoder, priority, unused, argument);
    decoder->vtable = data_02059c18;
    decoder->last = 0;
    decoder->first = decoder->last;
    decoder->scanline = 168;
    decoder->budget_frames = 5;
    decoder->interval_frames = 2;
    decoder->current_frame = 0;
    return decoder;
}

ArchiveMemoryDecoder *ArchiveMemoryDecoder_Destroy(ArchiveMemoryDecoder *decoder)
{
    decoder->vtable = data_02059c18;
    GameTask_DestroyBase((GameTask *)decoder);
    return decoder;
}

ArchiveMemoryDecoder *ArchiveMemoryDecoder_Delete(ArchiveMemoryDecoder *decoder)
{
    decoder->vtable = data_02059c18;
    GameTask_DestroyBase((GameTask *)decoder);
    GameHeap_Delete(decoder);
    return decoder;
}

static inline int ArchiveMemoryDecoder_GetVCount(void)
{
    return *(volatile u16 *)0x04000006;
}

void ArchiveMemoryDecoder_Update(ArchiveMemoryDecoder *decoder)
{
    if (!data_02060b2c[15] &&
        (ArchiveMemoryDecoder_GetVCount() < decoder->scanline || ArchiveMemoryDecoder_GetVCount() >= 192)) {
        decoder->current_frame = 0;
    } else if (decoder->current_frame >= decoder->interval_frames) {
        decoder->current_frame = decoder->budget_frames;
    } else {
        decoder->current_frame++;
        if (decoder->current_frame != decoder->budget_frames + 1) return;
    }
    do {
        if (!decoder->first) {
            decoder->current_frame = 0;
            return;
        }
        ArchiveMemoryDecoder_Process(decoder);
    } while (!data_02060b2c[15] &&
        (ArchiveMemoryDecoder_GetVCount() < decoder->scanline || ArchiveMemoryDecoder_GetVCount() >= 192));
}

u32 ArchiveMemoryDecoder_Prepare(ArchiveMemoryDecoder *decoder, ArchiveCompressedRequest *request)
{
    u32 blocks;
    request->read.cursor = request->read.destination;
    ArchiveCodec_ReadHeader(&request->read.cursor, &blocks, &request->output_size);
    request->blocks = blocks;
    request->compressed_state = 1;
    return request->output_size;
}

void ArchiveMemoryDecoder_Queue(ArchiveMemoryDecoder *decoder, ArchiveCompressedRequest *request, void *destination)
{
    request->destination = destination;
    request->write_cursor = request->destination;
    request->compressed_state = 2;
    request->read.next = 0;
    if (decoder->last) {
        decoder->last->next = &request->read;
        decoder->last = &request->read;
    } else {
        decoder->last = &request->read;
        decoder->first = decoder->last;
    }
    ArchiveMemoryDecoder_Process(decoder);
}

void ArchiveMemoryDecoder_Wait(ArchiveMemoryDecoder *decoder, ArchiveCompressedRequest *request)
{
    ArchiveReadRequest *cursor;
    for (cursor = decoder->first; cursor; cursor = cursor->next) {
        if (cursor == &request->read) break;
    }
    if (cursor) {
        while (request->compressed_state) ArchiveMemoryDecoder_Process(decoder);
    }
}

void ArchiveMemoryDecoder_Process(ArchiveMemoryDecoder *decoder)
{
    ArchiveCompressedRequest *request = (ArchiveCompressedRequest *)decoder->first;
    request->read.cursor += 2;
    if ((u32)request->write_cursor >= 0x05000000 && (u32)request->write_cursor < 0x07000400) {
        func_0202bfbc(&request->read.cursor, &request->write_cursor);
    } else {
        func_0202bf10(&request->read.cursor, &request->write_cursor);
    }
    request->compressed_state = 3;
    --request->blocks;
    if (!request->blocks) {
        request->compressed_state = 0;
        decoder->first = request->read.next;
        request->read.next = 0;
        if (!decoder->first) decoder->last = 0;
    }
}
