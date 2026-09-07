#include <game/field_animation.h>
#include <game/heap.h>

extern void func_0202cbd4(void *destination, int value, u32 size);

FieldSpriteAnimation *FieldSpriteAnimation_Init(FieldSpriteAnimation *state, void *field_system)
{
    GameSpriteAnimation *animation;
    func_0202cbd4(state, 0, sizeof(*state));
    state->field_system = field_system;
    animation = GameHeap_New(sizeof(*animation), 0, 0, 0);
    if (animation) animation = GameSpriteAnimation_Initialize(animation, state, 128, 4096);
    state->animation = animation;
    return state;
}

FieldSpriteAnimation *FieldSpriteAnimation_Destroy(FieldSpriteAnimation *state)
{
    GameSpriteAnimation *animation = state->animation;
    if (animation) {
        GameSpriteAnimation_Finalize(animation);
        GameHeap_Delete(animation);
    }
    return state;
}
