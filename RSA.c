#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <stdlib.h>

void eea(mpz_t gcd, mpz_t inv, mpz_t e, mpz_t r0){ // r1 < r2
    mpz_t r2, r1, ri, // r2 = r_i-2, r1 = r_i-1, ri = r_i, r0
        q1, // q_i-1
        t2, t1, t; // t2 = t_i-2, t1 = t_i-1, t = t_i

    mpz_init(r2);
    mpz_init(r1);
    mpz_init(ri);
    mpz_init(q1);
    mpz_init(t2);
    mpz_init(t1);
    mpz_init(t);

    mpz_set_ui(t2, 0);
    mpz_set_ui(t1, 1);

    mpz_set(r1, e);
    mpz_set(r2, r0);

    // Variaveis exclusivas para as contas do eea
    mpz_t temp_sub, temp_mul;
    mpz_init(temp_sub);
    mpz_init(temp_mul);

    do
    {
        // Quociente: q1 = r2 / r1
        mpz_fdiv_q(q1, r2, r1);

        // Atualizar r_i: ri = r2 % r1
        mpz_mod(ri, r2, r1);

        // Atualizar t: t = t2 - q1 * t1
        mpz_mul(t, q1, t1);  // t = q1 * t1
        mpz_sub(t, t2, t);   // t = t2 - t

        // Deslocar as variáveis para a próxima iteração
        mpz_set(r2, r1);  // r2 = r1
        mpz_set(r1, ri);  // r1 = ri
        mpz_set(t2, t1);  // t2 = t1
        mpz_set(t1, t);   // t1 = t

    } while (mpz_cmp_ui(ri, 0)); // r1 > 0


    if(mpz_cmp_ui(t2, 0) < 0) { // se t2 < 0
        mpz_add(t2, t2, r0);    // t2 += r0
    }

    mpz_set(gcd, r2);
    mpz_set(inv, t2);

    mpz_clear(r2);
    mpz_clear(r1);
    mpz_clear(ri);
    mpz_clear(q1);
    mpz_clear(t2);
    mpz_clear(t1);
    mpz_clear(t);
    mpz_clear(temp_sub);
    mpz_clear(temp_mul);
}

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
    int nbits = 512;

    // 1. ESCOLHER PRIMOS GRANDES p, q
    mpz_t p, q;
    mpz_init(p);
    mpz_init(q);

    prime(p, nbits/2);
    prime(q, nbits/2);

    // gmp_printf("p:\n%Zd\nq:\n%Zd\n\n", p, q);

    // 2. CALCULAR n
    mpz_t n;
    mpz_init(n);
    mpz_mul(n, p, q);
    // gmp_printf("n:\n%Zd\n\n", n);

    // printf("num de bits da chave: %d\n\n", mpz_sizeinbase(n, 2));

    // 3. CALCULAR phi(n) = (p-1)*(q-1) 
    mpz_t phi;
    mpz_init(phi);
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_mul(phi, p, q);

    // gmp_printf("phi(n):\n%Zd\n\n", phi);

    // 4-5. SELECIONAR O EXPOENTE PÚBLICO e em {2,..., phi(n)-1}
    mpz_t gcd, inv, e;
    mpz_init(gcd);
    mpz_init(inv);
    mpz_init(e);

    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL)^rand());

    do {
        do {
            mpz_urandomm(e, state, phi);  // gera de 0 a phi-1...
        }while(mpz_cmp_ui(e, 1) <= 0);
        eea(gcd, inv, e, phi);
    } while(mpz_cmp_ui(gcd, 1) != 0);

    gmp_randclear(state);

    // gmp_printf("e:\n%Zd\ninv:\n%Zd\nmdc:\n%Zd\n\n", e, inv, gcd);


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