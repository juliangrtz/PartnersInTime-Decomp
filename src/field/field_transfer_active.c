/*
 * Transfer state (overlay 0, 0x020660F8-0x02066124).
 *
 * Whether a field transfer is running.
 */

#include <game/field_system.h>
int FieldSystem_IsTransferActive(FieldSystem *system) {
    FieldPendingTransfer *transfer = system->pending_transfer;
    return transfer && transfer->flags.phase == 2;
}
