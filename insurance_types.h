// Data Structures for Insurance Events
// ============================================================================

#ifndef INSURANCE_TYPES_H
#define INSURANCE_TYPES_H

#include <time.h>
#include <stdint.h>

// Event Types for Insurance Transactions
typedef enum {
    ENROLLMENT,
    PREMIUM_PAYMENT,
    PREAUTH_REQUEST,
    CLAIM_SUBMISSION,
    CLAIM_DECISION
} EventType;

// Insurance Payload Structure
typedef struct {
    char policy_id[32];
    char member_id[32];
    EventType event_type;
    char provider_id[32];
    double amount;
    char diagnosis_code[16];
    char notes[256];
} InsurancePayload;

// Block Structure
typedef struct Block {
    uint32_t block_id;
    time_t timestamp;
    InsurancePayload payload;
    char prev_hash[65];
    char hash[65];
    uint32_t nonce;
    struct Block *next;
} Block;

// Blockchain Structure
typedef struct {
    Block *head;
    Block *tail;
    uint32_t length;
    uint32_t difficulty;
} Blockchain;

// Utility Functions
const char* event_type_to_string(EventType type);
EventType string_to_event_type(const char* str);

#endif // INSURANCE_TYPES_H