/*		Máquina de Pilha Aritmética
*	Integrantes: 	Lucas Mota Ribeiro (12/0016885)
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
#define MAX_LEN 255 //Maior tamanho possível de input
#define MAX_STACK 255 // Maximo tamanho da pilha


/* Pilha*/
typedef struct opStack{
    char ops[MAX_STACK];
    int top;
}opStack;

opStack * newStack()
{
    opStack *stack =(opStack*) malloc(sizeof(opStack));
    stack->top = -1;
    return stack;
}

void push(opStack *stack, char op)
{
    if (stack->top != MAX_STACK) {
        stack->ops[++stack->top] = op;
    } else {
        printf("Pilha ultrapassou tamanho máximo.\n");
    }
}

char pop(opStack *stack)
{
    char op = 0;
    if  (stack->top != -1) {
        op = stack->ops[stack->top];
        stack->ops[stack->top--] = 0;
    }
    return op;
}
char check(opStack *stack)
{
    char op = 0;
    if  (stack->top != -1) {
        op = stack->ops[stack->top];
    }
    return op;
}

int cmpPrecedence(char a, char b)
{
    enum { // Menor valor, maior precedência
        OPEN = 2,
        CLOSE = 2,
        MULT = 5,
        DIV = 5,
        ADD = 6,
        SUB = 6
    };
    int val_a, val_b;
    switch (a) {
        case '(':
            val_a = OPEN;
            break;
        case ')':
            val_a = CLOSE;
            break;
        case '*':
            val_a = MULT;
            break;
        case '/':
            val_a = DIV;
            break;
        case '+':
            val_a = ADD;
            break;
        case '-':
            val_a = SUB;
            break;
    }
    switch (b) {
        case '(':
            val_b = OPEN;
            break;
        case ')':
            val_b = CLOSE;
            break;
        case '*':
            val_b = MULT;
            break;
        case '/':
            val_b = DIV;
            break;
        case '+':
            val_b = ADD;
            break;
        case '-':
            val_b = SUB;
            break;
    }
    return val_a - val_b;
}

/* Converte a equação infixada para uma posfixada pelo algoritmo Shunting-yard.*/
char* infixedToRPN(char *input)
{
    char *buffer = (char*) malloc(sizeof(char) * MAX_LEN * 2 - 1);
    opStack *stack = newStack();
    char* cont = input;
    int offset = 0;
    // printf("char: %d\n", *cont);
    while(*cont != 0) { // Percorre a string
        int readNumber = 0;
        while(*cont >= '0' && *cont <= '9') { // Pega todos o digitos em sequencia
            int toWrite = MAX_LEN * 2 - 1 - offset;
            int written = snprintf(buffer + offset, toWrite, "%c", *cont);
            if (toWrite < written) {
                break;
            }
            offset += written;
            readNumber = 1;
            cont++;
        }
        snprintf(buffer + offset, MAX_LEN * 2 - 1 - offset, " ");
        offset++;

        switch (*cont) {
            case '(':
                push(stack, '(');
            break;
            case ')':
                while (check(stack) != '(' && check(stack) != 0){
                    int toWrite = MAX_LEN * 2 - 1 - offset;
                    char ch = pop(stack);
                    int written = snprintf(buffer + offset, toWrite, "%c ", ch);
                    offset += written;
                }
                char final = pop(stack);
                if(final != '('){
                    printf("Casamento de parenteses falho\n");
                }
            break;
            case '*':
                while (check(stack) != 0 && cmpPrecedence('*', check(stack)) >= 0) {
                    int toWrite = MAX_LEN * 2 - 1 - offset;
                    int written = snprintf(buffer + offset, toWrite, "%c ", pop(stack));
                    offset += written;
                }
                push(stack, '*');
            break;
            case '/':
                while (check(stack) != 0 && cmpPrecedence('/', check(stack)) >= 0) {
                    int toWrite = MAX_LEN * 2 - 1 - offset;
                    int written = snprintf(buffer + offset, toWrite, "%c ", pop(stack));
                    offset += written;
                }
                push(stack, '/');
            break;
            case '+':
                while (check(stack) != 0 && cmpPrecedence('-', check(stack)) <= 0) {
                    int toWrite = MAX_LEN * 2 - 1 - offset;
                    int written = snprintf(buffer + offset, toWrite, "%c ", pop(stack));
                    offset += written;
                }
                push(stack, '+');
            break;
            case '-':
                while (check(stack) != 0 && cmpPrecedence('-', check(stack)) <= 0) {
                    int toWrite = MAX_LEN * 2 - 1 - offset;
                    int written = snprintf(buffer + offset, toWrite, "%c ", pop(stack));
                    offset += written;
                }
                push(stack, '-');

            break;
        }
        cont++;
    }
    while (check(stack) != 0) {
        char ch = pop(stack);
        int toWrite = MAX_LEN * 2 - 1 - offset;
        // printf("BEFORE: str: %s\n", buffer);
        int written = snprintf(buffer + offset, toWrite, "%c ", ch);
        // printf("AFTER: str: %s\n", buffer);
        // printf("str: %s| off: %d\n", buffer, offset);
        offset += written;
    }
    // printf("str: %s| off: %d\n", buffer, offset);
    return buffer;
}

/* Checa se a entrada é valida sintáticamente*/
int checkIfValid(char *input)
{
    char *contador = input;
    int ret = 1; //Retorno
    while(*contador != 0) {
        char ch = *contador++;
        if (!(ch == '(' || ch == ')' || ch == '+' || ch == '-' || ch == '*' || ch == '/' || (ch >= '0' && ch <= '9'))){
            // Encontrou caractere desconhecido.
            ret = 0;
            break;
        }
    }
    return ret;
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
/* Método seguro de ler input sem dar overflow */
int readInput(char *buffer, size_t bufsiz)
{
    char format[16];
    snprintf(format, sizeof(format), "%%%zus", bufsiz - 1);
    return scanf(format, buffer);
}
/* Pega o input de usuário pela entrada padrão */
char* getUserInput(int *valid)
{
    char *buffer = (char*) malloc(sizeof(char) * MAX_LEN);
    printf("Entre com a equação(%d caracteres no máximo)\n", MAX_LEN);
    *valid = readInput(buffer, MAX_LEN);
    return buffer;
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

int main(int argc, char*argv[])
{
    // Cecebe a equação.
    int inputSize = 0;
    char *input = concatInput(argc, argv, &inputSize);
    if (inputSize > 0) {
        printf("Recebeu equação por linha de comando: %s\n", input);
    } else {
        int validUserInp = 0; // 1 é OK, errado caso contrário.
        while (!validUserInp) {
            input = getUserInput(&validUserInp);
            printf("%d - valid\n", validUserInp);
            if (validUserInp && removeEspacos(input) == 0) {
                printf("%d - valid2\n", validUserInp);
                validUserInp = 0;
            }
        }
    }
    int valid = checkIfValid(input);
    char *rpn = infixedToRPN(input);
    printf("Posfixada: %s\n", rpn);

    free(input); // Libera espaço de memória.
	return 0;
}
