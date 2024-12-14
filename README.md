# Criptografia RSA
Trabalho desenvolvido para a disciplina de Introdução a Criptografia.

## Descrição
O projeto utiliza a biblioteca externa GMP para manipulação de grandes inteiros.
- `key.c`:  utilizado para facilitar a manipulação das chaves;
- `key_genetor.c`: rotina de geração de chaves públicas e privadas de tamanho `512`, `1024` e `2048` para o RSA;
- `RSA.c`: exponenciação generalizada que equivale à operação de cifragem e decifragem do RSA.

## Execução 
Para executar, basta utilizar o script em `Makefile` e gerar os dois programas:
```
make key_gen
make RSA
```
e realizar suas chamadas:
```
./key_gen
./rsa
```
Ou compilar e linkar manualmente os arquivos `key_generator.c` e `key.c` e `rsa.c` e `key.c` (observe que é necessário indicar a utilização da biblioteca externa GMP com a flag `-lgmp`):
```
gcc key_generator.c key.c -o key_gen -lgmp
gcc rsa.c key.c -o rsa -lgmp
```
e realizar sua chamada:
```
./key_gen
./rsa
``` 

## Autores
- Beatriz Rogers Tripoli Barbosa
- Laura Naomi Seto