#include <game/field_party_manager.h>
extern void func_0202cbd4(void *, int, u32);
extern void func_020488bc(void *, int, u32, FieldPartyController *(*)(FieldPartyController *));
extern void func_020489e0(void *, int, u32, FieldPartyController *(*)(FieldPartyController *),
                          FieldPartyController *(*)(FieldPartyController *));
FieldPartyManager *FieldPartyManager_InitializeStorage(FieldPartyManager *manager)
{
    func_020489e0(manager->parties, 2, sizeof(FieldPartyController), FieldParty_InitializeStorage,
                  FieldParty_DestroyBackups);
    func_0202cbd4(manager, 0, sizeof(*manager));
    return manager;
}
FieldPartyManager *FieldPartyManager_DestroyStorage(FieldPartyManager *manager)
{
    func_020488bc(manager->parties, 2, sizeof(FieldPartyController), FieldParty_DestroyBackups);
    return manager;
}
void FieldPartyManager_SaveSnapshot(FieldPartyManager *manager, FieldPartyManagerSnapshot *snapshot)
{
    snapshot->flags.active_party = manager->bits.active_party;
    snapshot->flags.unknown_07 = manager->bits.unknown_07;
    snapshot->flags.unknown_08_11 = manager->bits.unknown_08_11;
    snapshot->flags.unknown_12 = manager->bits.unknown_12;
    snapshot->flags.unknown_13 = manager->bits.unknown_13;
    snapshot->flags.unknown_14 = manager->bits.unknown_14;
    snapshot->flags.unknown_16 = manager->bits.unknown_16;
    FieldParty_SaveSnapshot(&manager->parties[0], &snapshot->parties[0]);
    FieldParty_SaveSnapshot(&manager->parties[1], &snapshot->parties[1]);
}
