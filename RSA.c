#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <stdlib.h>

#include "key.h"

void sm(T_key *key, mpz_t base, mpz_t result) 
{
    mpz_t percorre, temp_AND;
    mpz_init(percorre); 
    mpz_init(temp_AND);

    // inicializar percorre com o msb do expoente
    mpz_set_ui(percorre, 1);
    mpz_mul_2exp(percorre, percorre, mpz_sizeinbase(key->exponent, 2) - 1);

    mpz_set_ui(result, 1);   

    while(mpz_cmp_ui(percorre, 0) > 0) {

        mpz_mul(result, result, result);
        mpz_mod(result, result, key->mod);

        mpz_and(temp_AND, key->exponent, percorre);

        if(mpz_cmp(temp_AND, percorre) == 0) {
            mpz_mul(result, result, base);
            mpz_mod(result, result, key->mod);
        }
        
        // shift right
        mpz_fdiv_q_2exp(percorre, percorre, 1);
    }

    mpz_clear(temp_AND);
    mpz_clear(percorre); 
}

int main(int argc, char const *argv[])
{
    T_key key;
    key_init(&key);

    mpz_t plain, cypher;
    mpz_init(plain);
    mpz_init(cypher);

    int base;
    printf("Base: ");
    scanf("%d", &base);

    if(base == 16){
        printf("Módulo: ");
        gmp_scanf("%Zx", key.mod);
        printf("Expoente: ");
        gmp_scanf("%Zx", key.exponent);
        printf("Mensagem: ");
        gmp_scanf("%Zx", plain);
    }
    else {
        printf("Módulo: ");
        gmp_scanf("%Zd", key.mod);
        printf("Expoente: ");
        gmp_scanf("%Zd", key.exponent);
        printf("Mensagem: ");
        gmp_scanf("%Zd", plain);
    }

    sm(&key, plain, cypher);

    if(base == 16){
        gmp_printf("Base:\n16\nMensagem:\n%Zx\n\n", cypher);
    }
    else {
        gmp_printf("Base:\n10\nMensagem:\n%Zd\n\n", cypher);
    }

    mpz_clear(plain);
    mpz_clear(cypher);
    key_clear(&key);

    return 0;
}
