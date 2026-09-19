/* Room-owned animation data and the script-facing start/wait operations. */
extern "C"
{
#include <game/battle_archive.h>
#include <game/heap.h>
}
#include <game/field_effect_animation.h>

extern "C"
{
    extern const u32 data_ov000_020bfbec[4];
    /* Native shared archive pathname: /FieldFx/FieldFxData.dat. */
    extern const char data_ov000_020c0e4c[];
    extern void func_0202cbd4(void *, int, u32);

    void FieldResources_LoadAnimations(FieldResourceContext *area)
    {
        for (int i = 0; i < 4; ++i)
            area->special_animations[i] = 0;
        for (int set = 0; set < 2; ++set)
        {
            int count = area->animation_counts[set];
            if (count > 0)
            {
                u8 **buffers =
                    (u8 **)GameHeap_NewArray(count * sizeof(u8 *), area->heaps.main, 0, 1);
                area->animation_buffers[set] = buffers;
            }
            else
                area->animation_buffers[set] = 0;
        }
        int count = area->animation_counts[0] + area->animation_counts[1];
        if (count > 0)
        {
            u32 size = count * sizeof(FieldArchiveRequest);
            area->reads =
                (FieldArchiveRequest *)GameHeap_NewArray(size, area->heaps.temporary, 0, 1);
            func_0202cbd4(area->reads, 0, size);
            area->read_count = 0;
            FieldArchiveRequest *request = area->reads;
            for (int set = 0; set < 2; ++set)
            {
                for (int i = 0; i < area->animation_counts[set]; ++i)
                {
                    u32 id = area->animation_ids[set][i];
                    request->read.entry = (u16)id;
                    request->read.limit = BattleArchive_GetEntrySize(
                        area->archive, area->archive->descriptors[6], (u16)id);
                    request->read.destination =
                        (u8 *)GameHeap_NewArray(request->read.limit, area->heaps.main, 0, 1);
                    area->animation_buffers[set][i] = request->read.destination;
                    for (int special = 0; special < 4; ++special)
                    {
                        if (id == data_ov000_020bfbec[special])
                        {
                            area->special_animations[special] = request->read.destination;
                            break;
                        }
                    }
                    BattleArchive_ReadAsync(area->archive, request, 0,
                                            area->archive->descriptors[6], data_ov000_020c0e4c,
                                            0x8000);
                    ++request;
                    ++area->read_count;
                }
            }
        }
    }

    int FieldResources_FinishAnimationReads(FieldResourceContext *area)
    {
        FieldArchiveRequest *request = area->reads;
        if (request)
        {
            for (int i = 0; i < area->read_count; ++i, ++request)
                if (request->read.state)
                    return 0;
            GameHeap_DeleteArray(area->reads);
            area->reads = 0;
        }
        return 1;
    }

    GameMatrixAnimationTrack *FieldArea_StartModelAnimation(FieldAreaContext *area, int set,
                                                            int index, int first, int second,
                                                            int third, int fourth, s16 x, s16 y,
                                                            s16 z, int overlap_priority, s16 speed)
    {
        const s16 *commands = (s16 *)((FieldResourceContext *)area)->animation_buffers[set][index];
        FieldRenderObject *first_model = first == -1 ? 0 : area->entities[first]->render_object;
        FieldRenderObject *second_model = second == -1 ? 0 : area->entities[second]->render_object;
        FieldRenderObject *third_model = third == -1 ? 0 : area->entities[third]->render_object;
        FieldRenderObject *fourth_model = fourth == -1 ? 0 : area->entities[fourth]->render_object;
        return FieldModelAnimation_Start(
            area->model_animation, area->flags.screen, commands, area->animation_models,
            first_model, second_model, third_model, fourth_model, x, y, z, overlap_priority, speed);
    }

    int FieldArea_IsModelAnimationActive(FieldAreaContext *area)
    {
        GameMatrixAnimationTrack *track = area->model_animation->tracks[area->flags.screen];
        int active = 0;
        if (track && !track->flags.bits.finished)
            active = 1;
        return active;
    }

    GameSpriteAnimationTrack *FieldArea_StartSpriteAnimation(FieldAreaContext *area, int slot,
                                                             int set, int index, s16 x, s16 y,
                                                             s16 speed)
    {
        return FieldSpriteAnimation_Start(
            area->sprite_animation, slot,
            (s16 *)((FieldResourceContext *)area)->animation_buffers[set][index], x, y, speed);
    }

    int FieldArea_IsSpriteAnimationActive(FieldAreaContext *area, int index)
    {
        GameSpriteAnimationTrack *track = area->sprite_animation->tracks[index];
        int active = 0;
        /* The native completion query normalizes the null-command flag first. */
        if (track && !(track->commands == 0 ? 1 : 0))
            active = 1;
        return active;
    }
}
