#ifndef KEY_H
#define KEY_H

typedef struct key {
    mpz_t mod;
    mpz_t exponent;
} T_key;

void key_init(T_key *key);

void key_clear(T_key *key);

void key_print(T_key *key);

#endif