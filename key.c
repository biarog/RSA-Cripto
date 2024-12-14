#include <stdio.h>
#include <gmp.h>

#include "key.h"

void key_init(T_key *key){
    mpz_init(key->mod);
    mpz_init(key->exponent);
}

void key_clear(T_key *key){
    mpz_clear(key->mod);
    mpz_clear(key->exponent);
}

void key_print(T_key *key){
    gmp_printf("Expoente:\n%Zd\n\nModulo:\n%Zd\n\n", key->exponent, key->mod);
}