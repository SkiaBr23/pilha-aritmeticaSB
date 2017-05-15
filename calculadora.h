/*		Máquina de Pilha Aritmética
*
*   Grupo 03
*	Integrantes: 	Arthur Jaber Costato  (13/0039993)
*					Paulo da Cunha Passos (10/0118577)
*					Rafael Dias da Costa  (12/0133253)
*					Lucas Mota Ribeiro 	  (12/0016885)
*					Miguel Montagner Filho (13/0127302) 
*			
*			
*   Arquivo: calculadora.h
*	Descrição: Arquivo de cabeçalho para funcoes utilizadas
*   na maquina de pilha aritmetica, suas macros e estruturas de dados.
*	
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


#define MAX_LEN 255 //Maior tamanho possível de input

/* Macro para destuir a pilha */
#define destruirPilha(var) \
 while(var != NULL){ \
	pilha_t* aux = var->prox; \
	free(var); \
	var = aux; \
}

#define TRUE 1
/* Tipo de dado com 64 bits que representa no máximo 48 bits */
typedef long long int int64;
typedef struct pilha_t pilha_t;

typedef struct uint48_t uint48_t;
struct __attribute__((packed)) uint48_t {
	int64 v:48;
};

/* Estrutura de dados da pilha */
struct pilha_t{
	uint48_t num;	// Valor numérico
	char op;	// Caractere da operação
	pilha_t* prox;	// Próximo elemento da pilha
};

/* Protótipo das funcoes presentes no arquivo calculadora.c */

void pushN(uint48_t num, pilha_t** prox);
void pushO(char op, pilha_t** prox);
uint48_t popN(pilha_t** pilha);
char popO(pilha_t** pilha);
void invertePilha(pilha_t** pilha);
void imprimePilha(pilha_t* pilha);
int precedencia(char a, char b);
void notacaoPosFixada(pilha_t** pilha);
uint48_t calculaPilha(pilha_t *entrada);
int removeEspacos(char* str);
char* concatInput(int ac, char **av, int *inpSize);
