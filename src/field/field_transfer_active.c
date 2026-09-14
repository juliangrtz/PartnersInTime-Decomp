#include <game/field_system.h>
int FieldSystem_IsTransferActive(FieldSystem *system) {
    FieldPendingTransfer *transfer = system->pending_transfer;
    return transfer && transfer->flags.phase == 2;
}
