#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <stdlib.h>

void prime(mpz_t number, int nbits) {
    gmp_randstate_t state;

    // inicialização das variáveis
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL)^rand());

    do {
        // geração de num aleatório
        mpz_urandomb(number, state, nbits); 
    } while(mpz_probab_prime_p(number, 15) == 0); //checar se é primo   

    // liberando memória 
    gmp_randclear(state);
}

int main(int argc, char const *argv[])
{
    // 512, 1024, 2048
    // gerar números aleatórios p e q até encontrar primos

    int nbits = 512;

    // 1. ESCOLHER PRIMOS GRANDES p, q
    mpz_t p, q;
    mpz_init(p);
    mpz_init(q);

    prime(p, nbits/2);
    prime(q, nbits/2);

    gmp_printf("%Zd\n%Zd\n", p, q);

    // 2. CALCULAR n
    mpz_t n;
    mpz_init(n);
    mpz_mul(n, p, q);
    gmp_printf("%Zd\n", n);

    printf("num de bits da chave: %d\n", mpz_sizeinbase(n, 2));

    // 3. CALCULAR phi(n) = (p-1)*(q-1) 
    mpz_t phi;
    mpz_init(phi);
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_mul(phi, p, q);

    gmp_printf("phi(n): %Zd\n", phi);

    // 4-5. SELECIONAR O EXPOENTE PÚBLICO e em {2,..., phi(n)-1}
    mpz_t gcd, inv, e;
    mpz_init(gcd);
    mpz_init(inv);
    mpz_init(e);

    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL)^rand());

    do {
        mpz_urandomm(e, state, phi);  // gera de 0 a phi-1...
        eea(gcd, inv, e, phi);
    } while(gcd != 1);

    gmp_randclear(state);

    gmp_printf("e: %Zd * %Zd = %Zd mod %Zd\n", e, inv, gcd, phi);


    // 


    // LIBERANDO MEMÓRIA
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(n);
    mpz_clear(phi);
    mpz_clear(gcd);
    mpz_clear(inv);
    mpz_clear(e);


    return 0;
}