// Blockchain Core Implementation
// ============================================================================

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "insurance_types.h"
#include "sha256.h"
#include "validation.h"

static Blockchain *blockchain = NULL;

// Calculate hash of a block
static void calculate_hash(Block *block, char *output) {
    SHA256_CTX ctx;
    uint8_t hash[SHA256_BLOCK_SIZE];
    char data[2048];
    
    snprintf(data, sizeof(data), "%u%ld%s%s%d%s%.2f%s%s%s%u",
             block->block_id,
             block->timestamp,
             block->payload.policy_id,
             block->payload.member_id,
             block->payload.event_type,
             block->payload.provider_id,
             block->payload.amount,
             block->payload.diagnosis_code,
             block->payload.notes,
             block->prev_hash,
             block->nonce);
    
    sha256_init(&ctx);
    sha256_update(&ctx, (uint8_t*)data, strlen(data));
    sha256_final(&ctx, hash);
    bytes_to_hex(hash, SHA256_BLOCK_SIZE, output);
}

// Proof of Work Mining
static int mine_block(Block *block, uint32_t difficulty) {
    char target[65] = {0};
    for (uint32_t i = 0; i < difficulty; i++) {
        target[i] = '0';
    }
    
    block->nonce = 0;
    do {
        block->nonce++;
        calculate_hash(block, block->hash);
    } while (strncmp(block->hash, target, difficulty) != 0);
    
    return 1;
}

// Initialize blockchain with genesis block
void blockchain_init(uint32_t difficulty) {
    blockchain = (Blockchain*)malloc(sizeof(Blockchain));
    blockchain->head = NULL;
    blockchain->tail = NULL;
    blockchain->length = 0;
    blockchain->difficulty = difficulty;
    
    // Create genesis block
    Block *genesis = (Block*)malloc(sizeof(Block));
    genesis->block_id = 0;
    genesis->timestamp = time(NULL);
    strcpy(genesis->payload.policy_id, "GENESIS");
    strcpy(genesis->payload.member_id, "SYSTEM");
    genesis->payload.event_type = ENROLLMENT;
    strcpy(genesis->payload.provider_id, "SYSTEM");
    genesis->payload.amount = 0.0;
    strcpy(genesis->payload.diagnosis_code, "N/A");
    strcpy(genesis->payload.notes, "Genesis Block - Health Insurance Blockchain");
    strcpy(genesis->prev_hash, "0");
    genesis->nonce = 0;
    genesis->next = NULL;
    
    mine_block(genesis, difficulty);
    
    blockchain->head = genesis;
    blockchain->tail = genesis;
    blockchain->length = 1;
}

// Add new block to blockchain
int blockchain_add_block(InsurancePayload payload) {
    if (!blockchain) {
        printf("Error: Blockchain not initialized\n");
        return 0;
    }
    
    Block *new_block = (Block*)malloc(sizeof(Block));
    new_block->block_id = blockchain->length;
    new_block->timestamp = time(NULL);
    new_block->payload = payload;
    strcpy(new_block->prev_hash, blockchain->tail->hash);
    new_block->nonce = 0;
    new_block->next = NULL;
    
    printf("Mining block %u...\n", new_block->block_id);
    mine_block(new_block, blockchain->difficulty);
    printf("Block mined! Hash: %s\n", new_block->hash);
    
    blockchain->tail->next = new_block;
    blockchain->tail = new_block;
    blockchain->difficulty = (blockchain->difficulty +1)%6;
    blockchain->length++;
    
    return 1;
}

// Verify blockchain integrity
int blockchain_verify() {
    if (!blockchain || !blockchain->head) {
        printf("Error: Empty blockchain\n");
        return 0;
    }
    
    Block *current = blockchain->head->next;
    int block_num = 1;
    
    while (current) {
        char calculated_hash[65];
        calculate_hash(current, calculated_hash);
        
        if (strcmp(current->hash, calculated_hash) != 0) {
            printf("Integrity check failed at block %d: Hash mismatch\n", block_num);
            return 0;
        }
        
        Block *prev = blockchain->head;
        for (int i = 0; i < block_num - 1; i++) {
            prev = prev->next;
        }
        if (strcmp(current->prev_hash, prev->hash) != 0) {
            printf("Integrity check failed at block %d: Chain linkage broken\n", block_num);
            return 0;
        }
        
        char target[65] = {0};
        for (uint32_t i = 0; i < blockchain->difficulty; i++) {
            target[i] = '0';
        }
        if (strncmp(current->hash, target, blockchain->difficulty) != 0) {
            printf("Integrity check failed at block %d: Invalid PoW\n", block_num);
            return 0;
        }
        
        current = current->next;
        block_num++;
    }
    
    printf("Blockchain verified successfully! All %u blocks are valid.\n", blockchain->length);
    return 1;
}

// View blockchain with masked sensitive data
void blockchain_view() {
    if (!blockchain || !blockchain->head) {
        printf("Blockchain is empty\n");
        return;
    }
    
    printf("\n=== HEALTH INSURANCE BLOCKCHAIN ===\n");
    printf("Total Blocks: %u | Difficulty: %u\n", blockchain->length, blockchain->difficulty);
    printf("Security: Sensitive data masked in display\n\n");
    
    Block *current = blockchain->head;
    while (current) {
        char masked_member_id[64];
        char masked_diagnosis[32];
        char masked_amount[32];
        
        mask_string(current->payload.member_id, masked_member_id, 3, 2);
        mask_string(current->payload.diagnosis_code, masked_diagnosis, 1, 1);
        mask_amount(current->payload.amount, masked_amount);
        
        printf("--- Block %u ---\n", current->block_id);
        printf("Timestamp: %s", ctime(&current->timestamp));
        printf("Policy ID: %s\n", current->payload.policy_id);
        printf("Member ID: %s (masked)\n", masked_member_id);
        printf("Event Type: %s\n", event_type_to_string(current->payload.event_type));
        printf("Provider ID: %s\n", current->payload.provider_id);
        printf("Amount: $%s (masked)\n", masked_amount);
        printf("Diagnosis Code: %s (masked)\n", masked_diagnosis);
        printf("Notes: %s\n", current->payload.notes);
        printf("Previous Hash: %s\n", current->prev_hash);
        printf("Hash: %s\n", current->hash);
        printf("Nonce: %u\n\n", current->nonce);
        
        current = current->next;
    }
}

// Save blockchain to file
void blockchain_save(const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        printf("Error: Could not open file for writing\n");
        return;
    }
    
    fwrite(&blockchain->difficulty, sizeof(uint32_t), 1, fp);
    fwrite(&blockchain->length, sizeof(uint32_t), 1, fp);
    
    Block *current = blockchain->head;
    while (current) {
        fwrite(&current->block_id, sizeof(uint32_t), 1, fp);
        fwrite(&current->timestamp, sizeof(time_t), 1, fp);
        fwrite(&current->payload, sizeof(InsurancePayload), 1, fp);
        fwrite(current->prev_hash, sizeof(char), 65, fp);
        fwrite(current->hash, sizeof(char), 65, fp);
        fwrite(&current->nonce, sizeof(uint32_t), 1, fp);
        current = current->next;
    }
    
    fclose(fp);
    printf("Blockchain saved to %s\n", filename);
}

// Load blockchain from file
void blockchain_load(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("No existing blockchain found. Starting fresh.\n");
        return;
    }
    
    blockchain = (Blockchain*)malloc(sizeof(Blockchain));
    fread(&blockchain->difficulty, sizeof(uint32_t), 1, fp);
    fread(&blockchain->length, sizeof(uint32_t), 1, fp);
    
    Block *prev = NULL;
    for (uint32_t i = 0; i < blockchain->length; i++) {
        Block *block = (Block*)malloc(sizeof(Block));
        fread(&block->block_id, sizeof(uint32_t), 1, fp);
        fread(&block->timestamp, sizeof(time_t), 1, fp);
        fread(&block->payload, sizeof(InsurancePayload), 1, fp);
        fread(block->prev_hash, sizeof(char), 65, fp);
        fread(block->hash, sizeof(char), 65, fp);
        fread(&block->nonce, sizeof(uint32_t), 1, fp);
        block->next = NULL;
        
        if (i == 0) {
            blockchain->head = block;
        } else {
            prev->next = block;
        }
        prev = block;
    }
    blockchain->tail = prev;
    
    fclose(fp);
    printf("Blockchain loaded from %s (%u blocks)\n", filename, blockchain->length);
}

// Cleanup blockchain memory
void blockchain_cleanup() {
    if (!blockchain) return;
    
    Block *current = blockchain->head;
    while (current) {
        Block *temp = current;
        current = current->next;
        free(temp);
    }
    free(blockchain);
    blockchain = NULL;
}

Blockchain* blockchain_get_instance() {
    return blockchain;
}