// Core Blockchain Operations
// ============================================================================

#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdint.h>
#include "insurance_types.h"

void blockchain_init(uint32_t difficulty);
int blockchain_add_block(InsurancePayload payload);
int blockchain_verify();
void blockchain_view();
void blockchain_save(const char *filename);
void blockchain_load(const char *filename);
void blockchain_cleanup();
Blockchain* blockchain_get_instance();

#endif // BLOCKCHAIN_H