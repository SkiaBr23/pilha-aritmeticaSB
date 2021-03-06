/*		Máquina de Pilha Aritmética
*
*   Grupo 03
*	Integrantes: 	Arthur Jaber Costato  (13/0039993)
*					Paulo da Cunha Passos (10/0118577)
*					Rafael Dias da Costa  (12/0133253)
*					Lucas Mota Ribeiro 	  (12/0016885)
*					Miguel Montagner Filho (13/0127302) 
*			
*   Arquivo: 3-02-main.c
*	Descrição:	Este programa receberá uma expressão aritmética com notação infixa via linha de comando
*			(com ou sem parênteses) (quatro operações básicas).
*			Então ele então a converterá para notação pós-fixa
*			Por fim, ele resolverá a expressão por meio de aritmética extendida.
*	
*/


/* Arquivo cabeçalho das funcoes/macros utilizadas e estruturas de dados */
#include "3-02-calculadora.h"

int main(int argc, char*argv[]){

	printf("=============================\nMaquina de Pilha Aritmetica\n=============================\n");
	char input[64];


	/* Caso a equacao tenha sido informada via parametro */
	if (argc > 1) {
		printf("Recebeu equacao por linha de comando:\n");
		for (int i = 1; i < argc; ++i)
		{
			/* Remocao de espacos para evitar erros de parsing */
			printf("%s ",argv[i] );
			removeEspacos(argv[i]);
		}
		printf("\n");

	/* Caso a equacao nao tenha sido informada via parametro, pede input do usuario */
	}else {
		printf("Insira a equacao desejada:\n");
		scanf("%[^\n]s",input);
		removeEspacos(input);
		argv[1] = malloc(sizeof(char)*strlen(input));
		strcpy(argv[1],input);
		argc++;
	}

	/* Declaracao de variaveis */
	int i;
	int negFlag = 0;
	uint48_t num;
	int parentesesAbertos = 0;
	int operacao = 5; // 0 = recebeu um número, 1 = recebeu '+', 2 = recebeu '-', 3 = recebeu um '*', 4 = recebeu um '/', 5 = recebeu um '(' ou é início da expressão, 6 = recebeu um ')'
	pilha_t* pilha = NULL;

	/* Processo de empilhamento da expressao */
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
			i--;// Caso não tenha espaço, faz com que a próxima interação volte para o mesmo argv
		}
	}
	
	/* Caso existam parenteses nao fechados na expressao */
	if(parentesesAbertos){
		printf("Por favor, verifique a quantidade de parênteses!");
		destruirPilha(pilha)
		return 0;
	}

	/* Caso a expressao termine com uma operacao */
	
	if(operacao != 0 && operacao != 6){
		printf("Por favor, as expressões devem terminar com um número ou ')'");
		destruirPilha(pilha)
		return 0;
	}

	printf("\n");

	/* Mostra representacao da pilha infixada */
	printf("Notacao Infixada da expressao: ");
	imprimePilha(pilha);

	/* Converte pilha infixada para notacao posfixada */
	notacaoPosFixada(&pilha);
	
	/* Mostra representacao da pilha posfixada */
	printf("Notacao Posfixada da expressao: ");
	imprimePilha(pilha);

	/* Calcula resultado da expressao */
	printf("\nResultado Final: %ld\n\n",(long)calculaPilha(pilha).v);

	/* Destroi pilha da execucao */
	destruirPilha(pilha)
	return 0;
}