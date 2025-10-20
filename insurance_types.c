// Implementation of Insurance Type Utilities
// ============================================================================

#include <string.h>
#include "insurance_types.h"

const char* event_type_to_string(EventType type) {
    switch(type) {
        case ENROLLMENT: return "ENROLLMENT";
        case PREMIUM_PAYMENT: return "PREMIUM_PAYMENT";
        case PREAUTH_REQUEST: return "PREAUTH_REQUEST";
        case CLAIM_SUBMISSION: return "CLAIM_SUBMISSION";
        case CLAIM_DECISION: return "CLAIM_DECISION";
        default: return "UNKNOWN";
    }
}

EventType string_to_event_type(const char* str) {
    if (strcmp(str, "ENROLLMENT") == 0) return ENROLLMENT;
    if (strcmp(str, "PREMIUM_PAYMENT") == 0) return PREMIUM_PAYMENT;
    if (strcmp(str, "PREAUTH_REQUEST") == 0) return PREAUTH_REQUEST;
    if (strcmp(str, "CLAIM_SUBMISSION") == 0) return CLAIM_SUBMISSION;
    if (strcmp(str, "CLAIM_DECISION") == 0) return CLAIM_DECISION;
    return ENROLLMENT;
}
