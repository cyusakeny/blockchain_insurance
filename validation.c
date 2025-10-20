// Input Validation Implementation
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int validate_id(const char *id) {
    if (strlen(id) == 0 || strlen(id) >= 32) {
        printf("Error: ID must be between 1-31 characters\n");
        return 0;
    }
    for (size_t i = 0; i < strlen(id); i++) {
        if (!isalnum(id[i]) && id[i] != '_' && id[i] != '-') {
            printf("Error: ID can only contain letters, numbers, underscores, and hyphens\n");
            return 0;
        }
    }
    return 1;
}

int validate_amount(double amount) {
    if (amount < 0.0) {
        printf("Error: Amount cannot be negative\n");
        return 0;
    }
    if (amount > 1000000.0) {
        printf("Error: Amount cannot exceed $1,000,000.00\n");
        return 0;
    }
    return 1;
}

int read_amount(double *amount) {
    char input[64];
    if (scanf("%63s", input) != 1) {
        return 0;
    }
    
    char *endptr;
    *amount = strtod(input, &endptr);
    
    if (endptr == input || *endptr != '\0') {
        printf("Error: Please enter a valid numeric amount\n");
        return 0;
    }
    
    return 1;
}

void mask_string(const char *input, char *output, int show_first, int show_last) {
    int len = strlen(input);
    if (len <= show_first + show_last) {
        strcpy(output, input);
        return;
    }
    
    int i;
    for (i = 0; i < show_first && i < len; i++) {
        output[i] = input[i];
    }
    
    for (; i < len - show_last; i++) {
        output[i] = '*';
    }
    
    for (; i < len; i++) {
        output[i] = input[i];
    }
    output[len] = '\0';
}

void mask_amount(double amount, char *output) {
    if (amount == 0.0) {
        strcpy(output, "0.00");
        return;
    }
    
    char temp[32];
    snprintf(temp, sizeof(temp), "%.2f", amount);
    
    int len = strlen(temp);
    int decimal_pos = -1;
    
    for (int i = 0; i < len; i++) {
        if (temp[i] == '.') {
            decimal_pos = i;
            break;
        }
    }
    
    for (int i = 0; i < len; i++) {
        if (i < decimal_pos - 2 && isdigit(temp[i])) {
            output[i] = '*';
        } else {
            output[i] = temp[i];
        }
    }
    output[len] = '\0';
}