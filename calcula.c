/*		Máquina de Pilha Aritmética
*	Integrantes: 	Arthur Jaber Costato (13/0039993)
*			
*			
*			
*
*	Descrição:	Este programa receberá uma expressão aritmética com notação infixa via linha de comando
*			(com ou sem parênteses) (quatro operações básicas).
*			Então ele então a converterá para notação pós-fixa
*			Por fim, ele resolverá a expressão por meio de aritmética extendida.
*	Estruturas de Dados:	
*	Macros:		
*	Funções:	
*	
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

/* Struct da pilha */
struct pilha_t{
	uint48_t num;	// Valor numérico
	char op;	// Caractere da operação
	pilha_t* prox;	// Próximo elemento da pilha
};


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
	char aux;
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
Função que pega uma pilha pós-fixada e calcula o valor
funcao destrutiva, pois altera a pilha de entrada
empilha os numeros até encontrar operacao, desempilha
a qtd necessaria, realiza o calculo e coloca na pilha o
novo resultado. caso dê tudo certo, é para finalizar com
o cursor que percorre a entrada == NULL e apenas um numero
na pilha
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

int main(int argc, char*argv[]){
	if(argc == 1){
		printf("Por favor, entre com a expressão na linha de comando!\n");
		return 0;
	}
	
	int i;
	int negFlag = 0;
	uint48_t num;
	int parentesesAbertos = 0;
	int operacao = 5; // 0 = recebeu um número, 1 = recebeu '+', 2 = recebeu '-', 3 = recebeu um '*', 4 = recebeu um '/', 5 = recebeu um '(' ou é início da expressão, 6 = recebeu um ')'
	pilha_t* pilha = NULL;
	for(i = 1; i < argc; i++){
		if(*argv[i] == '('){
			if(operacao == 0 || operacao == 6){ // Se recebeu um número ou outro parêntese fechando antes de '(', então multiplica
				pushO('*', &pilha);
			}
			argv[i]++;	// Avança ponteiro de argv para o próximo caractere
			parentesesAbertos++; // Incrementa a quantidade de parênteses abertos
			pushO('(', &pilha);
			operacao = 5;	// Sinaliza que última operação acrescenada na pilha foi um '('
		}else if(*argv[i] == ')'){
			if(operacao == 0 || operacao == 6){ // Somente fecha parênteses se anteriormente havia um número ou outro parêntese fechando
				argv[i]++;	// Avança ponteiro de argv para o próximo caractere
				parentesesAbertos--; // Decrementa a quantidade de parênteses abertos
				pushO(')', &pilha);
				operacao = 6;	// Sinaliza que última operação acrescenada na pilha foi um ')'
			}else if(operacao == 5){ // Se abrir e fechar um parentese, remove ele e adiciona um zero (pois dentro deles está vazio == 0)
				popO(&pilha);
				num.v = 0;
				pushN(num,&pilha);
			}else{	// Não pode ter + ) ou - ) ou * ) ou / )
				printf("Por favor, não coloque + ) ou - ) ou * ) ou / )\n");
				destruirPilha(pilha)
				return 0;
			}
		}else if(*argv[i] == '+'){
			argv[i]++;	// Avança ponteiro de argv para o próximo caractere
			if(operacao == 0 || operacao == 6){ // Se já tiver recebido um '+' ou um '-' ou um '*' ou um '/' ou um '(' ou é início da expressão, não tem porquê acrescentar outro '+' na pilha    
				pushO('+', &pilha);
				operacao = 1;// Sinaliza que última operação acrescenada na pilha foi um '+'
			}
		}else if(*argv[i] == '-'){
			argv[i]++;	// Avança ponteiro de argv para o próximo caractere
			if(operacao == 1){ // Se tiver recebido um '+', retira ele e coloca um '-'
				popO(&pilha);
				pushO('-', &pilha);
				operacao = 2;// Sinaliza que última operação acrescenada na pilha foi um '-'
			}
			else if(operacao == 2){ // Se já tiver recebido um '-', retira ele e coloca um '+'
				popO(&pilha);
				if((pilha == NULL) || (pilha->op == '(')){
					operacao = 5; // Não coloca +, só indica que operação anterior foi '(' ou início da expressão
				}else if(pilha->op == '*'){
					operacao = 3; // Não coloca +, só indica que operação anterior foi '*'
				}else if(pilha->op == '/'){
					operacao = 4; // Não coloca +, só indica que operação anterior foi '/'
				}else{
					pushO('+', &pilha);
					operacao = 1;// Sinaliza que última operação acrescenada na pilha foi um '+'
				}
			}else{
				pushO('-', &pilha);
				operacao = 2;// Sinaliza que última operação acrescenada na pilha foi um '-'
			}
		}else if(*argv[i] == '*'){
			argv[i]++;	// Avança ponteiro de argv para o próximo caractere
			if(operacao != 3){ // Se já tiver recebido um '*', não tem porquê acrescentar outro '*' na pilha
				if(operacao == 0 || operacao == 6){
					// Acrescenta na pilha somente se tiver recebido um número ou ')'
					pushO('*', &pilha);
					operacao = 3;// Sinaliza que última operação acrescenada na pilha foi um '*'
				}else{ // Não pode colocar + * ou - * ou / * ou ( *
					printf("Por favor, não coloque + * ou - * ou / * ou ( *\n");
					destruirPilha(pilha)
					return 0;
				}
			}
		}else if(*argv[i] == '/'){
			argv[i]++;	// Avança ponteiro de argv para o próximo caractere
			if(operacao == 0 || operacao == 6){
				pushO('/', &pilha);
				operacao = 4;
			}else{ // Não pode colocar + / ou - / ou * / ou / / ou ( /
				printf("Por favor, não coloque + / ou - / ou * / ou / / ou ( /\n");
				destruirPilha(pilha)
				return 0;
			}
		}else if((*argv[i]>='0' && *argv[i]<='9')){
			if(operacao == 2){
				if(pilha->prox->op != 0 && pilha->prox->op != ')'){ // Se não tiver um número ou ')' antes do '-', retira-se o '-' e faz o número der negativo
																	// Ou seja, no caso de "1 * -3" ou "1 / -3" ou "1 + (-3)"
					popO(&pilha);
					negFlag = 1;
				}
			}else if(operacao == 0 || operacao == 6){ // Se recebeu outro número ou estiver fechando parênteses ')', então multiplica
				pushO('*', &pilha);
			}
			
			num.v = 0;
			while((*argv[i]>='0' && *argv[i]<='9')){
				num.v = 10*num.v + (*argv[i]-48);
				argv[i]++;
			}
			if(*argv[i] == ',' || *argv[i] == '.'){
				printf("Por favor, entre somente com números inteiros");
				destruirPilha(pilha)
				return 0;
			}
			if(negFlag){
				num.v *= -1;
				negFlag = 0;
			}
			pushN(num, &pilha);
			operacao = 0;	// Sinaliza que foi acrescentado um número na pilha
		}else{
			printf("Por favor, entre somente com números inteiros ou operações: + - * / ( )");
			destruirPilha(pilha)
			return 0;
		}
		if(*argv[i] != '\0'){
			i--;// Caso não tenha espaço, faz com que a próxima ineração volte para o mesmo argv
		}
	}
	
	if(parentesesAbertos){
		printf("Por favor, verifique a quantidade de parênteses!");
		destruirPilha(pilha)
		return 0;
	}
	
	if(operacao != 0 && operacao != 6){
		printf("Por favor, as expressões devem terminar com um número ou ')'");
		destruirPilha(pilha)
		return 0;
	}
	imprimePilha(pilha);

	notacaoPosFixada(&pilha);
	

	imprimePilha(pilha);

	printf("\nResultado Final: %ld\n",(long)calculaPilha(pilha).v);

	destruirPilha(pilha)
	return 0;
}