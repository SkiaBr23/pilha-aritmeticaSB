/*		Máquina de Pilha Aritmética
*
*   Grupo 03
*	Integrantes: 	Arthur Jaber Costato  (13/0039993)
*					Paulo da Cunha Passos (10/0118577)
*					Rafael Dias da Costa  (12/0133253)
*			
*			
*   Arquivo: calculadora.c
*	Descrição:	Arquivo que contem as funcoes gerais da maquina de pilha aritmetica
*  
*	
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "calculadora.h"



/* Função para criação da pilha inicializando com um número */
void pushN(uint48_t num, pilha_t** prox){
	pilha_t* pilha = (pilha_t*)malloc(sizeof(pilha_t));
	if(pilha == NULL){
		printf("Erro! Falta de memória!");
		//destruirPilha(*prox)
		// Como finalizar o programa?
	}
	pilha->num = num;
	pilha->op = 0;
	pilha->prox = *prox;
	*prox = pilha;
}

/* Função para criação da pilha inicializando com uma operação */
void pushO(char op, pilha_t** prox){
	pilha_t* pilha = (pilha_t*)malloc(sizeof(pilha_t));
	if(pilha == NULL){
		printf("Erro! Falta de memória!");
		//destruirPilha(*prox)
		// Como finalizar o programa?
	}
	pilha->num.v =  0;
	pilha->op = op;
	pilha->prox = *prox;
	*prox = pilha;
}

/* Retira topo da pilha e retorna como número */
uint48_t popN(pilha_t** pilha){
	uint48_t num = (*pilha)->num;
	pilha_t* prox = (*pilha)->prox;
	free(*pilha);
	(*pilha) = prox;
	return num;
}

/* Retira topo da pilha e retorna como operação */
char popO(pilha_t** pilha){
	char op = (*pilha)->op;
	pilha_t* prox = (*pilha)->prox;
	free(*pilha);
	(*pilha) = prox;
	return op;
}

/* Inverte a ordem dos elementos de uma pilha */
void invertePilha(pilha_t** pilha){
	pilha_t* pilhaAux = NULL;
	
	// Ordena pilha do início para o fim
	while(*pilha != NULL){
		if((*pilha)->op == 0){
			pushN(popN(pilha),&pilhaAux);
		}else{
			pushO(popO(pilha),&pilhaAux);
		}
	}
	*pilha = pilhaAux;
	pilhaAux = NULL;
}

/* Imprime a pilha */
void imprimePilha(pilha_t* pilha){
	while(pilha != NULL){
		if(pilha->op == 0){
			printf("%ld ", (long)pilha->num.v);
		}else{
			printf("%c ", pilha->op);
		}
		pilha = pilha->prox;
	}
	printf("\n");
}

/* Dentre duas operações aritiméticas, indica qual operação precede a outra */
int precedencia(char a, char b){
	int precA = 0;
	int precB = 0;

	switch(a){
		case '*':
			precA = 3;
			break;
		case '/':
			precA = 3;
			break;
		case '+':
			precA = 1;
			break;
		case '-':
			precA = 1;
			break;
		case '(':
			precA = 3;
			break;
	}
	
	switch(b){
		case '*':
			precB = 2;
			break;
		case '/':
			precB = 2;
			break;
		case '+':
			precB = 1;
			break;
		case '-':
			precB = 1;
			break;
		case '(':
			precB = 0;
			break;
	}

	return (precB < precA);
}

/* Converte pilha para uma notação posfixada */
void notacaoPosFixada(pilha_t** pilha){
	pilha_t* pilhaAux = NULL;
	pilha_t* pilhaPos = NULL;
	invertePilha(pilha);

	pushO('(', &pilhaAux);
	while(*pilha != NULL){
		if((*pilha)->op == 0){
			pushN(popN(pilha),&pilhaPos);
		}
		else if((*pilha)->op == '('){
			pushO(popO(pilha), &pilhaAux);
		}
		else if((*pilha)->op == ')'){
			do{
				if (pilhaAux->op != '(')
					pushO(popO(&pilhaAux), &pilhaPos); // duvida de erro
			}while(pilhaAux->op != '(');
			popO(&pilhaAux);
			popO(pilha);
		}
		else if((*pilha)->op == '+' || (*pilha)->op == '-' || (*pilha)->op == '*' || (*pilha)->op == '/'){
			while(TRUE){		
					if(precedencia((*pilha)->op, pilhaAux->op)){
						pushO(popO(pilha), &pilhaAux);
						break;
					}
					else{
						pushO(popO(&pilhaAux),&pilhaPos);
					}
			}
		}
	}
	do{
		if (pilhaAux->op != '(')
			pushO(popO(&pilhaAux), &pilhaPos); // duvida de erro
		}while(pilhaAux->op != '(');

	*pilha = pilhaPos;
	pilhaPos = NULL;
	pilhaAux = NULL;
	invertePilha(pilha);
}

/*
Função que pega uma pilha pós-fixada e calcula o valor.
A funcao eh destrutiva, pois altera a pilha de entrada e
empilha os numeros até encontrar operacao, desempilha
a qtd necessaria, realiza o calculo e coloca na pilha o
novo resultado. Será finalizada com o cursor que percorre
a entrada == NULL e apenas um numero na pilha
*/

uint48_t calculaPilha(pilha_t *entrada) {
	pilha_t *pilhaNumeros = NULL;
	while(entrada != NULL) {
		uint48_t resp, val1, val2; //necessarios para realizar operacoes;
		if(entrada->op) { //significa ser uma operacao e nao um numero
			resp.v = 0; //zera para evitar erros
			switch(entrada->op) {
				case '+': 	val1 = popN(&pilhaNumeros);
							val2 = popN(&pilhaNumeros);
							resp.v = val1.v + val2.v;
							pushN(resp, &pilhaNumeros);
							break;

				case '-': 	val1 = popN(&pilhaNumeros);
							val2 = popN(&pilhaNumeros);
							resp.v = val2.v - val1.v;
							pushN(resp, &pilhaNumeros);
							break;

				case '*': 	val1 = popN(&pilhaNumeros);
							val2 = popN(&pilhaNumeros);
							resp.v = val1.v * val2.v;
							pushN(resp, &pilhaNumeros);
							break;

				case '/': 	val1 = popN(&pilhaNumeros);
							val2 = popN(&pilhaNumeros);
							resp.v = val2.v / val1.v;
							pushN(resp, &pilhaNumeros);
							break;

			}
		} else { //signfica ser um numero
			pushN(entrada->num, &pilhaNumeros); //insere o numero na pilha
		}
		entrada = entrada->prox; //anda para prox simbolo da pilha
	}

	return pilhaNumeros->num;

}


/* Retira todos os espaços de uma string e retorna a quantidade de caracteres final.*/
int removeEspacos(char* str)
{
    int size = 0;
    char *caracter = str;
    char *contador = str;
    while(*contador != 0) {
        *caracter = *contador++; // pega o próximo caracter.
        size++;
        if(*caracter != ' ') {
            caracter++;
        } else {
            size--;
        }
    }
    *caracter = 0; // Nulo no final
    return size;
}

/* Contatena o input por linha de comando e retorna a string e inpSize recebe o tamanho da entrada.*/
char* concatInput(int ac, char **av, int *inpSize)
{
    char *buffer = (char*) malloc(sizeof(char) * MAX_LEN);
    buffer[0] = 0;
    int offset = 0;
    *inpSize = 0;
    while(av++, --ac) {
       int toWrite = MAX_LEN - offset;
       // Copia cada argv no buffer mais o offset do argv anterior.
       int written = snprintf(buffer + offset, toWrite, "%s", *av);
       if (toWrite < written) {
           *inpSize += written;
           break;
       }
       *inpSize += written;
       offset += written;
    }
    return buffer;
}

