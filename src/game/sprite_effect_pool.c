#include <game/sprite_effect.h>

extern u8 data_02056658[];
extern void func_0202cbd4(void *destination, int value, u32 size);

GameSpriteEffectPool *GameSpriteEffectPool_Init(GameSpriteEffectPool *pool, u32 count)
{
    u32 size = count * sizeof(GameSpriteEffect);
    pool->entries = GameHeap_NewArray(size, 0, data_02056658, 0);
    func_0202cbd4(pool->entries, 0, size);
    pool->count.raw = count;
    pool->active_list.head_marker = (GameSpriteEffect *)0x11111111;
    pool->active_list.first = (GameSpriteEffect *)&pool->active_list.last;
    pool->active_list.last = (GameSpriteEffect *)&pool->active_list.head_marker;
    pool->active_list.tail_marker = (GameSpriteEffect *)-1;
    pool->free_list.head_marker = (GameSpriteEffect *)0x11111111;
    pool->free_list.first = (GameSpriteEffect *)&pool->free_list.last;
    pool->free_list.last = (GameSpriteEffect *)&pool->free_list.head_marker;
    pool->free_list.tail_marker = (GameSpriteEffect *)-1;
    if (pool->count.raw) {
        GameSpriteEffect *current = pool->entries;
        GameSpriteEffect *next;
        s16 remaining;
        pool->free_list.first = current;
        current->previous = (GameSpriteEffect *)&pool->free_list.head_marker;
        next = current + 1;
        for (remaining = pool->count.raw - 1; remaining > 0; ++next) {
            current->next = next;
            next->previous = current;
            --remaining;
            ++current;
        }
        current->next = (GameSpriteEffect *)&pool->free_list.last;
        pool->free_list.last = next - 1;
    }
    return pool;
}

GameSpriteEffectPool *GameSpriteEffectPool_Destroy(GameSpriteEffectPool *pool)
{
    GameHeap_DeleteArray(pool->entries);
    return pool;
}
