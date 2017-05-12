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
#define destruirPilha(var) while(var != NULL){\
	pilha_t* aux = var->prox;\
	free(var);\
	var = aux;\
}

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
		destruirPilha(*prox)
		// Como finalizar o programa?
	}
	pilha->num = num;
	pilha->prox = *prox;
	*prox = pilha;
}

/* Função para criação da pilha inicializando com uma operação */
void pushO(char op, pilha_t** prox){
	pilha_t* pilha = (pilha_t*)malloc(sizeof(pilha_t));
	if(pilha == NULL){
		printf("Erro! Falta de memória!");
		destruirPilha(*prox)
		// Como finalizar o programa?
	}
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

int main(int argc, char*argv[]){
	if(argc == 1){
		printf("Por favor, entre com a expressão na linha de comando!\n");
		return 0;
	}
	
	int i;
	int negFlag = 0;
	uint48_t num;
	int parentesesAbertos = 0;
	int operacao = 5; // 0 = recebeu um número, 1 = recebeu '+', 2 = recebeu '-', 3 = recebeu um '*', 4 = recebeu um '/', 5 = recebeu um '(', 6 = recebeu um ')'
	pilha_t* pilha = NULL;
	for(i = 1; i < argc; i++){
		if(*argv[i] == '('){
			if(operacao == 0){ // Se recebeu um número, então multiplica
				pushO('*', &pilha);
			}
			argv[i]++;	// Avança ponteiro de argv para o próximo caractere
			parentesesAbertos++; // Incrementa a quantidade de parênteses abertos
			pushO('(', &pilha);
			operacao = 5;	// Sinaliza que última operação acrescenada na pilha foi um '('
		}else if(*argv[i] == ')'){
			argv[i]++;	// Avança ponteiro de argv para o próximo caractere
			parentesesAbertos--; // Decrementa a quantidade de parênteses abertos
			pushO(')', &pilha);
			operacao = 6;	// Sinaliza que última operação acrescenada na pilha foi um ')'
		}else if(*argv[i] == '+'){
			argv[i]++;	// Avança ponteiro de argv para o próximo caractere
			if(operacao != 1 && operacao != 2){ // Se já tiver recebido um '+' ou um '-', não tem porquê acrescentar outro '+' na pilha
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
				pushO('+', &pilha);
				operacao = 1;// Sinaliza que última operação acrescenada na pilha foi um '+'
			}else{
				pushO('-', &pilha);
				operacao = 2;// Sinaliza que última operação acrescenada na pilha foi um '-'
			}
		}else if(*argv[i] == '*'){
			argv[i]++;	// Avança ponteiro de argv para o próximo caractere
			if(operacao == 1 || operacao == 2 || operacao == 4 || operacao == 5){ // Não pode colocar + * ou - * ou / * ou ( *
				printf("Por favor, não coloque + * ou - * ou / * ou ( *\n");
				destruirPilha(pilha)
				return 0;
			}else if(operacao != 3){ // Se já tiver recebido um '*', não tem porquê acrescentar outro '*' na pilha
				// Acrescenta na pilha somente se tiver recebido um número ou ')'
				pushO('*', &pilha);
				operacao = 3;// Sinaliza que última operação acrescenada na pilha foi um '*'
			}
		}else if(*argv[i] == '/'){
			argv[i]++;	// Avança ponteiro de argv para o próximo caractere
			if(operacao == 1 || operacao == 2 || operacao == 3 || operacao == 4){ // Não pode colocar + / ou - / ou * / ou / /
				printf("Por favor, não coloque + / ou - / ou * / ou / /\n");
				destruirPilha(pilha)
				return 0;
			}else{
				pushO('/', &pilha);
				operacao = 4;
			}
		}else if((*argv[i]>='0' && *argv[i]<='9')){
			if(operacao == 2){ // Se tiver um símbolo de negativo, retira ele e seta flag para alterar o sinal no final
				popO(&pilha);
				negFlag = 1;
			}else if(operacao == 6){ // Se estiver fechando parênteses ')', então multiplica
				pushO('*', &pilha);
			}else if(operacao == 1){ // Se tiver entrado com um símbolo de positivo e antes tiver um '(' ou NULL, então o remove
				if((pilha->prox == NULL) || (pilha->prox->op == '(')){
					popO(&pilha);
				}
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
				num *= -1;
				negFlag = 0;
			}
			pushN(num, &pilha);
			operacao = 0;	// Sinaliza que foi acrescentado um número na pilha
		}else if(*argv[i] != '\0'){
			i--;// Caso não tenha espaço, faz com que a próxima ineração volte para o mesmo argv
		}else{
			printf("Por favor, entre somente com números inteiros ou operações: + - * / ( )");
			destruirPilha(pilha)
			return 0;
		}
	}
	
	if(parentesesAbertos){
		printf("Por favor, verifique a quantidade de parênteses!");
		destruirPilha(pilha)
		return 0;
	}
	
	destruirPilha(pilha)
	return 0;
}
