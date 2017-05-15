-- Trabalho 2 C/ISO / Grupo 03 --

Membros do grupo:

Arthur Jaber Costato  		13/0039993  (estrutura de dados e leitura da expressao na pilha)
Paulo da Cunha Passos 	    10/0118577  (conversao da pilha infixada para posfixada)
Lucas Mota Ribeiro 			12/0016885	(conversao da pilha infixada para posfixada, validacao dos parametros)
Miguel Montagner Filho      13/0127302  (calculo da expressao posfixada)
Rafael Dias da Costa	 	12/0133253  (parsing da equacao via usuario, modularizacao e documentacao do codigo)

--> Neste trabalho foi implementado um resolvedor de expressões aritméticas baseado em uma máquina de pilha utilizando a notação polonesa reversa, que recebe uma equação de inteiros por linha de comando ou dentro do programa, caso não tenham sido passados os parâmetros.
O usuario pode informar a equacao via parametro do programa ou diretamente na execucao.

--> O programa permite as 4 operacoes basicas (soma, subtracao, divisao, multiplicacao), com ou sem parenteses. O uso de parenteses como multiplicadores tambem funciona (Ex: (3)4 == 3*4).

--> Para utilizar numeros de 48 bits, foi implementada uma estrutura de atributo "uint_48" que usa os primeiros 48bits de uma variavel 64 bits. Ela se encontra no arquivo calculadora.h (linha 37). Essa tecnica funciona de forma generica. 

--> Detalhes sobre o processo de empilhamento e calculo da expressao estao presentes diretamente no codigo como comentarios.

--> Para compilar e executar o código, deve ser chamado o seguinte comando:

gcc -std=c99 -Wall main.c calculadora.c -o calculadora -lm

--> Exemplo de execucao via argumento (As aspas duplas sao necessarias para evitar erro de obtencao de parametro):

./calculadora "7 + 3-2+ 8 * (2/2) - 4/2"

SAIDA ESPERADA:

=============================
Maquina de Pilha Aritmetica
=============================
Recebeu equacao por linha de comando
7 + 3-2+ 8 * (2/2) - 4/2

Notacao Infixada da expressao: 2 / 4 - ) 2 / 2 ( * 8 + 2 - 3 + 7 
Notacao Posfixada da expressao: 7 3 + 2 - 8 2 2 / * + 4 2 / - 

Resultado Final: 14


--> Exemplo de execucao via entrada do usuario (Nao necessario utilizar aspas duplas):

./calculadora

SAIDA ESPERADA:

=============================
Maquina de Pilha Aritmetica
=============================
Insira a equacao desejada:
7 + 3-2+ 8 * (2/2) - 4/2

Notacao Infixada da expressao: 2 / 4 - ) 2 / 2 ( * 8 + 2 - 3 + 7 
Notacao Posfixada da expressao: 7 3 + 2 - 8 2 2 / * + 4 2 / - 

Resultado Final: 14





