-- Trabalho 2 C/ISO / Grupo 03 --

Membros do grupo:

Arthur Jaber Costato  		13/0039993  (estrutura de dados e leitura da expressao na pilha)
Paulo da Cunha Passos 	    10/0118577  (conversao da pilha infixada para posfixada)
Lucas Mota Ribeiro 			12/0016885	(conversao da pilha infixada para posfixada, validacao dos parametros)
Miguel Montagner Filho      13/0127302  (calculo da expressao posfixada)
Rafael Dias da Costa	 	12/0133253  (parsing da equacao via usuario, modularizacao e documentacao do codigo)

Neste trabalho foi implementado um resolvedor de expressões aritméticas baseado em uma máquina de pilha utilizando a notação polonesa reversa, que recebe uma equação de inteiros por linha de comando ou dentro do programa, caso não tenham sido passados os parâmetros.


Para compilar e executar o código, executar:

gcc -std=c99 -Wall main.c calculadora.c -o calculadora -lm


Exemplo de execucao via argumento (As aspas duplas sao necessarias para evitar erro de obtencao de parametro):

./calculadora "7 + 3-2+ 8 * (2/2) - 4/2"

Exemplo de execucao via entrada do usuario (Nao necessario utilizar aspas duplas):

./calculadora

7 + 3-2+ 8 * (2/2) - 4/2







