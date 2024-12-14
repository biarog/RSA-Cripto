#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <stdlib.h>
#include <key.h>

void eea(mpz_t gcd, mpz_t inv, mpz_t e, mpz_t r0){ // r1 < r2
    mpz_t r1, ri, // r2 = r_i-2, r1 = r_i-1, ri = r_i, r0
        q1, // q_i-1
        t1, t; // t2 = t_i-2, t1 = t_i-1, t = t_i

    mpz_init(r1);
    mpz_init(ri);
    mpz_init(q1);
    mpz_init(t1);
    mpz_init(t);

    mpz_set_ui(inv, 0);
    mpz_set_ui(t1, 1);

    mpz_set(r1, e);
    mpz_set(gcd, r0);

    do
    {
        // Quociente: q1 = r2 / r1
        mpz_fdiv_q(q1, gcd, r1);

        // Atualizar r_i: ri = r2 % r1
        mpz_mod(ri, gcd, r1);

        // Atualizar t: t = t2 - q1 * t1
        mpz_mul(t, q1, t1);  // t = q1 * t1
        mpz_sub(t, inv, t);   // t = t2 - t

        // Deslocar as variáveis para a próxima iteração
        mpz_set(gcd, r1);  // r2 = r1
        mpz_set(r1, ri);  // r1 = ri
        mpz_set(inv, t1);  // t2 = t1
        mpz_set(t1, t);   // t1 = t

    } while (mpz_cmp_ui(ri, 0)); // r1 > 0


    if(mpz_cmp_ui(inv, 0) < 0) { // se t2 < 0
        mpz_add(inv, inv, r0);    // t2 += r0
    }

    mpz_clear(r1);
    mpz_clear(ri);
    mpz_clear(q1);
    mpz_clear(t1);
    mpz_clear(t);
}

void prime_generator(mpz_t number, int nbits) {
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

void key_generator(int nbits, int base)
{
    T_key kpub, kpriv;
    key_init(&kpub);
    key_init(&kpriv);

    // 1. ESCOLHER PRIMOS GRANDES p, q
    mpz_t p, q;
    mpz_init(p);
    mpz_init(q);

    prime_generator(p, nbits/2);
    prime_generator(q, nbits/2);

    // 2. CALCULAR n
    mpz_mul(kpriv.mod, p, q);
    mpz_set(kpub.mod, kpriv.mod);

    // 3. CALCULAR phi(n) = (p-1)*(q-1) 
    mpz_t phi, temp_p, temp_q;
    mpz_init(phi);
    mpz_init(temp_p);
    mpz_init(temp_q);

    mpz_sub_ui(temp_p, p, 1);
    mpz_sub_ui(temp_q, q, 1);
    mpz_mul(phi, temp_p, temp_q);

    mpz_clear(temp_p);
    mpz_clear(temp_q);

    // 4-5. SELECIONAR O EXPOENTE PÚBLICO e em {2,..., phi(n)-1}
    mpz_t gcd;
    mpz_init(gcd);

    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL)^rand());

    do {
        do {
            mpz_urandomm(kpub.exponent, state, phi);  // gera de 0 a phi-1...
        }while(mpz_cmp_ui(kpub.exponent, 1) <= 0);
        eea(gcd, kpriv.exponent, kpub.exponent, phi);
    } while(mpz_cmp_ui(gcd, 1) != 0);

    if(base == 16){
        gmp_printf("16\n%Zx\n%Zx\n%Zx\n%Zx\n%Zx\n", kpub.mod, kpub.exponent, kpriv.exponent, p, q);
    }
    else {
        gmp_printf("10\n%Zd\n%Zd\n%Zd\n%Zd\n%Zd\n", kpub.mod, kpub.exponent, kpriv.exponent, p, q);
    }

    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(gcd);
    mpz_clear(phi);
    gmp_randclear(state);
    key_clear(&kpub);
    key_clear(&kpriv);
}

int main(int argc, char const *argv[])
{
    int nbits, base;
    printf("Número de bits da chave (512, 1024, 2048): ");
    scanf("%d", &nbits);

    printf("Base de representação da saída: ");
    scanf("%d", &base);

    key_generator(nbits, base);
    return 0;
}
