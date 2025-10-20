#ifndef VALIDATION_H
#define VALIDATION_H

int validate_id(const char *id);
int validate_amount(double amount);
int read_amount(double *amount);
void mask_string(const char *input, char *output, int show_first, int show_last);
void mask_amount(double amount, char *output);

#endif // VALIDATION_H
