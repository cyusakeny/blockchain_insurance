// CLI Implementation


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "insurance_types.h"
#include "cli.h"
#include "blockchain.h"
#include "validation.h"

void cli_enroll() {
    InsurancePayload payload = {0};
    
    printf("Enter Policy ID: ");
    scanf("%31s", payload.policy_id);
    if (!validate_id(payload.policy_id)) return;
    
    printf("Enter Member ID: ");
    scanf("%31s", payload.member_id);
    if (!validate_id(payload.member_id)) return;
    
    printf("Enter Provider ID: ");
    scanf("%31s", payload.provider_id);
    
    payload.event_type = ENROLLMENT;
    payload.amount = 0.0;
    strcpy(payload.diagnosis_code, "N/A");
    
    printf("Enter Notes: ");
    getchar();
    fgets(payload.notes, 255, stdin);
    payload.notes[strcspn(payload.notes, "\n")] = 0;
    
    blockchain_add_block(payload);
}

void cli_pay() {
    InsurancePayload payload = {0};
    
    printf("Enter Policy ID: ");
    scanf("%31s", payload.policy_id);
    
    printf("Enter Member ID: ");
    scanf("%31s", payload.member_id);
    
    printf("Enter Payment Amount: $");
    if (!read_amount(&payload.amount)) return;
    if (!validate_amount(payload.amount)) return;
    
    payload.event_type = PREMIUM_PAYMENT;
    strcpy(payload.provider_id, "INSURER");
    strcpy(payload.diagnosis_code, "N/A");
    strcpy(payload.notes, "Premium payment received");
    
    blockchain_add_block(payload);
}

void cli_preauth() {
    InsurancePayload payload = {0};
    
    printf("Enter Policy ID: ");
    scanf("%31s", payload.policy_id);
    
    printf("Enter Member ID: ");
    scanf("%31s", payload.member_id);
    
    printf("Enter Provider ID: ");
    scanf("%31s", payload.provider_id);
    
    printf("Enter Estimated Amount: $");
    if (!read_amount(&payload.amount)) return;
    if (!validate_amount(payload.amount)) return;
    
    printf("Enter Diagnosis Code: ");
    scanf("%15s", payload.diagnosis_code);
    
    payload.event_type = PREAUTH_REQUEST;
    
    printf("Enter Decision Notes (APPROVED/DENIED/PARTIAL): ");
    getchar();
    fgets(payload.notes, 255, stdin);
    payload.notes[strcspn(payload.notes, "\n")] = 0;
    
    blockchain_add_block(payload);
}

void cli_help() {
    Blockchain *bc = blockchain_get_instance();
    
    printf("\n=== HEALTH INSURANCE BLOCKCHAIN CLI ===\n\n");
    printf("Commands:\n");
    printf("  enroll       - Create new policy/member enrollment\n");
    printf("  pay          - Record premium payment\n");
    printf("  preauth      - Submit pre-authorization request\n");
    printf("  claim submit - Submit insurance claim\n");
    printf("  claim decide - Record claim decision\n");
    printf("  view         - Display entire blockchain (sensitive data masked)\n");
    printf("  verify       - Verify blockchain integrity\n");
    printf("  save         - Save blockchain to file\n");
    printf("  load         - Load blockchain from file\n");
    printf("  help         - Show this help message\n");
    printf("  exit         - Exit program\n\n");
}

void cli_run() {
    char command[64];
    
    printf("=== HEALTH INSURANCE BLOCKCHAIN SYSTEM ===\n");
    printf("Initializing blockchain with difficulty 4...\n\n");
    
    blockchain_init(4);
    cli_help();
    
    while (1) {
        printf("> ");
        if (scanf("%63s", command) != 1) break;
        
        if (strcmp(command, "enroll") == 0) {
            cli_enroll();
        } else if (strcmp(command, "pay") == 0) {
            cli_pay();
        } else if (strcmp(command, "preauth") == 0) {
            cli_preauth();
        } else if (strcmp(command, "claim") == 0) {
            char subcmd[16];
            scanf("%15s", subcmd);
            if (strcmp(subcmd, "submit") == 0) {
                cli_claim_submit();
            } else if (strcmp(subcmd, "decide") == 0) {
                cli_claim_decide();
            } else {
                printf("Unknown claim subcommand. Use 'submit' or 'decide'\n");
            }
        } else if (strcmp(command, "view") == 0) {
            blockchain_view();
        } else if (strcmp(command, "verify") == 0) {
            blockchain_verify();
        } else if (strcmp(command, "save") == 0) {
            blockchain_save("blockchain.dat");
        } else if (strcmp(command, "load") == 0) {
            blockchain_load("blockchain.dat");
        } else if (strcmp(command, "help") == 0) {
            cli_help();
        } else if (strcmp(command, "exit") == 0) {
            printf("Saving blockchain before exit...\n");
            blockchain_save("blockchain.dat");
            break;
        } else {
            printf("Unknown command. Type 'help' for available commands.\n");
        }
    }
    
    blockchain_cleanup();
}
void cli_claim_submit() {
    InsurancePayload payload = {0};
    
    printf("Enter Policy ID: ");
    scanf("%31s", payload.policy_id);
    
    printf("Enter Member ID: ");
    scanf("%31s", payload.member_id);
    
    printf("Enter Provider ID: ");
    scanf("%31s", payload.provider_id);
    
    printf("Enter Claim Amount: $");
    if (!read_amount(&payload.amount)) return;
    if (!validate_amount(payload.amount)) return;
    
    printf("Enter Diagnosis Code: ");
    scanf("%15s", payload.diagnosis_code);
    
    payload.event_type = CLAIM_SUBMISSION;
    
    printf("Enter Notes: ");
    getchar();
    fgets(payload.notes, 255, stdin);
    payload.notes[strcspn(payload.notes, "\n")] = 0;
    
    blockchain_add_block(payload);
}
void cli_claim_decide() {
    InsurancePayload payload = {0};
    
    printf("Enter Policy ID: ");
    scanf("%31s", payload.policy_id);
    
    printf("Enter Member ID: ");
    scanf("%31s", payload.member_id);
    
    printf("Enter Provider ID: ");
    scanf("%31s", payload.provider_id);
    
    printf("Enter Approved Amount: $");
    scanf("%lf", &payload.amount);
    
    printf("Enter Diagnosis Code: ");
    scanf("%15s", payload.diagnosis_code);
    
    payload.event_type = CLAIM_DECISION;
    
    printf("Enter Decision Notes (APPROVED/DENIED/PARTIAL): ");
    getchar();
    fgets(payload.notes, 255, stdin);
    payload.notes[strcspn(payload.notes, "\n")] = 0;
    
    blockchain_add_block(payload);
}