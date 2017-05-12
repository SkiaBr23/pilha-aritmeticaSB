/*		M�quina de Pilha Aritm�tica
*	Integrantes: 	Arthur Jaber Costato (13/0039993)
*					
*					
*					
*
*	Descri��o:	Este programa receber� uma express�o aritm�tica com nota��o infixa via linha de comando
*					(com ou sem par�nteses) (quatro opera��es b�sicas).
*				Ent�o ele ent�o a converter� para nota��o p�s-fixa
*				Por fim, ele resolver� a express�o por meio de aritm�tica extendida.
*	Estruturas de Dados:	
*	Macros:		
*	Fun��es:	
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

typedef struct pilha_t pilha_t;
typedef union dado_t dado_t;

/* Union para salvar o valor do dado */
union dado_t{
	double numero;	// Pode ser um n�mero
	char operacao;	// Ou uma opera��o
};

/* Struct da pilha */
struct pilha_t{
	dado_t dado;	// Valor do dado
	pilha_t* prox;	// Pr�ximo elemento da pilha
};

/* Fun��o para cria��o da pilha inicializando com um n�mero */
void pushN(double dado, pilha_t** prox){
	pilha_t* pilha = (pilha_t*)malloc(sizeof(pilha_t));
	pilha->dado.numero = dado;
	pilha->prox = *prox;
	*prox = pilha;
}

/* Fun��o para cria��o da pilha inicializando com uma opera��o */
void pushO(char dado, pilha_t** prox){
	pilha_t* pilha = (pilha_t*)malloc(sizeof(pilha_t));
	pilha->dado.operacao = dado;
	pilha->prox = *prox;
	*prox = pilha;
}

/* Retira topo da pilha e retorna como n�mero */
double popN(pilha_t** pilha){
	double dado = (*pilha)->dado.numero;
	pilha_t* prox = (*pilha)->prox;
	free(*pilha);
	(*pilha) = prox;
	return dado;
}

/* Retira topo da pilha e retorna como opera��o */
char popO(pilha_t** pilha){
	char dado = (*pilha)->dado.operacao;
	pilha_t* prox = (*pilha)->prox;
	free(*pilha);
	(*pilha) = prox;
	return dado;
}

int main(int argc, char*argv[]){
	if(argc == 1){
		printf("Por favor, entre com a express�o na linha de comando!\n");
		return 0;
	}
	
	int i, k, flag;
	int inteiro,decimal;
	double num;
	int parentesesAbertos = 0;
	int operacao = 5; // 0 = recebeu um n�mero, 1 = recebeu '+', 2 = recebeu '-', 3 = recebeu um '*', 4 = recebeu um '/', 5 = recebeu um '(', 6 = recebeu um ')'
	pilha_t* pilha = NULL;
	for(i = 1; i < argc; i++){
		if(*argv[i] == '('){
			if(operacao == 0){ // Se recebeu um n�mero, ent�o multiplica
				pushO('*', &pilha);
			}
			operacao = 5;
			argv[i]++;	// Avan�a ponteiro de argv para o pr�ximo caractere
			parentesesAbertos++; // Incrementa a quantidade de par�nteses abertos
			pushO('(', &pilha);
		}else if(*argv[i] == ')'){
			operacao = 6;
			argv[i]++;	// Avan�a ponteiro de argv para o pr�ximo caractere
			parentesesAbertos--; // Decrementa a quantidade de par�nteses abertos
			pushO(')', &pilha);
		}else if(*argv[i] == '+'){
			argv[i]++;	// Avan�a ponteiro de argv para o pr�ximo caractere
			if(operacao != 1 && operacao != 2){ // Se j� tiver recebido um '+' ou um '-', n�o tem porqu� acrescentar outro '+' na pilha
				pushO('+', &pilha);
				operacao = 1;
			}
		}else if(*argv[i] == '-'){
			argv[i]++;	// Avan�a ponteiro de argv para o pr�ximo caractere
			if(operacao == 1){ // Se tiver recebido um '+', retira ele e coloca um '-'
				popO(&pilha);
				pushO('-', &pilha);
				operacao = 2;
			}
			else if(operacao == 2){ // Se j� tiver recebido um '-', retira ele e coloca um '+'
				popO(&pilha);
				pushO('+', &pilha);
				operacao = 1;
			}else{
				//if(operacao == 5){ // Se recebeu '(' ou est� iniciando, acrescenta o n�mero 0.0 antes do '-'
				//	pushN(0.0, &pilha);
				//}
				pushO('-', &pilha);
				operacao = 2;
			}
		}else if(*argv[i] == '*'){
			argv[i]++;	// Avan�a ponteiro de argv para o pr�ximo caractere
			if(operacao == 1 || operacao == 2 || operacao == 4){ // N�o pode colocar + * ou - * ou / *
				printf("Por favor, n�o coloque + * ou - * ou / *\n");
				destruirPilha(pilha)
				return 0;
			}else if(operacao != 3){ // Se j� tiver recebido um '*', n�o tem porqu� acrescentar outro '*' na pilha
				pushO('*', &pilha);
				operacao = 3;
			}
		}else if(*argv[i] == '/'){
			argv[i]++;	// Avan�a ponteiro de argv para o pr�ximo caractere
			if(operacao == 1 || operacao == 2 || operacao == 3 || operacao == 4){ // N�o pode colocar + / ou - / ou * / ou / /
				printf("Por favor, n�o coloque + / ou - / ou * / ou / /\n");
				destruirPilha(pilha)
				return 0;
			}else{
				pushO('/', &pilha);
				operacao = 4;
			}
		}else if((*argv[i]>='0' && *argv[i]<='9')||(*argv[i] == ',')||(*argv[i] == '.')){
			if(operacao == 6){ // Se estiver fechando par�nteses, ent�o multiplica
				pushO('*', &pilha);
			}
			operacao = 0;
			inteiro = 0;
			decimal = 0;
			k = 0;
			flag = 1;
			while((*argv[i]>='0' && *argv[i]<='9')||(*argv[i] == ',')||(*argv[i] == '.')){
				if(*argv[i] == ',' || *argv[i] == '.'){
					if(flag == 0){
						printf("Por favor, N�o coloque duas v�rgulas/pontos no mesmo m�mero");
						destruirPilha(pilha)
						return 0;
					}
					flag = 0;
				}else if(flag){
					inteiro = inteiro*10 + (*argv[i]-48);
				}else{
					decimal = decimal*10 + (*argv[i]-48);
					k++;
				}
				argv[i]++;
			}
			num = inteiro + decimal/pow(10,k);
			pushN(num, &pilha);
		}
		
		if(*argv[i] != '\0')   i--;	// Caso n�o tenha espa�o, faz com que a pr�xima inera��o volte para o mesmo argv
	}
	
	if(parentesesAbertos){
		printf("Por favor, verifique os par�nteses!");
		destruirPilha(pilha)
		return 0;
	}
	
	/* Testando as fun��es push e pop:
	pilha_t* pilha = NULL;
	pushN(1.0, &pilha);
	pushO('+', &pilha);
	pushN(1.0, &pilha);
	
	printf("%.2lf %c %.2lf", popN(&pilha), popO(&pilha), popN(&pilha));
	*/
	
	destruirPilha(pilha)
	return 0;
}

