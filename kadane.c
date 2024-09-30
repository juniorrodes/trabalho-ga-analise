/**
 * Este algoritmo foi criado para a disciplina de Análise e Projeto de Algoritmos,
 * o algoritmo é usado para achar a submatrix com a maior soma dentro de uma matrix.
 *
 * O algoritmo usa o algoritmo de kadane para achar a solução, este algoritmo consegue
 * achar o sub-array continuo com a maior soma dentro um array, neste caso nós conseguimos
 * fazer a soma das linhas da matrix e fazer um push em uma lista, aplicando o algoritmo de
 * kadane nesta lista nós conseguimos achar a solução do problema
 * 
 *  No código da solução as seguintes funções, com suas devidas responsabilidades:
 *      main : função principal, controla o fluxo do programa.
 *      parser : transforma a matriz bidimensional em vetor unidimensional, retorna também
 *          o tamanho da mesma.
 *      readLine : lê a linha do arquivo e cria um buffer para o valor.
 *      assertResponse : função para conferência do resultado obtido com o resultado no
 *          arquivo de conferência proposto pela professora.
 *      maxSubMatrizSum: calcula soma máxima usando técnica do prefixo e chama a função
 *          kadane.
 *      kadane : encontra a soma máxima do subvetor de uma coluna específica.
 *      freeList : libera memória
 *      newListItem : cria novo item em uma lista, com valor específico.
 *      push : adiciona novo valor ao final da lista
 *
 *  O motor da solução encontra-se nas funções maxSubMatrizSum e Kadane, onde é feita a iteração
 *  das colunas, somando-as, para finalmente aplicar o algoritmo de Kadane em cada linha, encontrando
 *  a soma máxima. 
 */

/***********************************/
/* José Luís Rodes Maciel Júnior   */
/* Fabíola Favero Seghetto         */
/***********************************/

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#define max(a,b) a > b ? a : b;

#define print(...) \
    printf(__VA_ARGS__);\
    fflush(stdout);

int *parser(FILE *f, int* matrix_size);
void readLine(FILE *f, char *buf);
int maxSubMatrixSum(int *matrix, int matrix_size);
void assertResponse(FILE *f, int v);

int main(int argc, char **argv) {
    if (argc < 2) {
        print("Missing values, please inform the number of the input file");
    }
    char file_name[100];
    sprintf(file_name, "./tests/in%s", argv[1]);
    //print("Opening %s\n", file_name);
    FILE *f = fopen(file_name, "r");
    int matrix_size = 0;
    int *matrix = parser(f, &matrix_size);
    sprintf(file_name, "./tests/out%s", argv[1]);
    f = fopen(file_name, "r");

    time_t t = clock();
    int max_sum = maxSubMatrixSum(matrix, matrix_size);
    //print("max sum is: %d\n", max_sum);
    t = clock() - t;
    print("%f\n", (((double)t)/CLOCKS_PER_SEC) * 1000);
    free(matrix);
    assertResponse(f, max_sum);
    return 0;
}

void assertResponse(FILE *f, int v){
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char buf[10] = {0};
    fread(buf, sizeof(char), size, f);
    int expected = atoi(buf);
    fclose(f);
    assert(expected == v);
}

int *parser(FILE *f, int* matrix_size) {
    char buf[1024];

    readLine(f, buf);
    *matrix_size = atoi(buf);
    //print("%d\r\n", MATRIX_SIZE);
    int *matrix = (int*)malloc(sizeof(int) * (*matrix_size * *matrix_size));

    for(int i = 0; i < *matrix_size; i++) {
        //readLine(f, buf);
        int j = 0;
        char num_str[5];
        char c;
        do{
        c = getc(f);
            int offset = 0;
            while(c != ' ' && c != '\n' && c != '\r' && c != EOF) {
                num_str[offset++] = c;
                c = getc(f);
            }
            num_str[offset] = '\0';
            int num = atoi(num_str);
            matrix[j + (*matrix_size * i)] = num;
            j++;
        }while (c != '\n' && c != EOF);
    }

    fclose(f);
    return matrix;
}

void readLine(FILE *f, char *buf) {
    char c;
    int offset = 0;
    fread(&c, 1, 1, f);
    while (c != '\n') {
        buf[offset++] = c;
        c = getc(f);
    }
    buf[offset] = '\0';
}

typedef struct List {
    int value;
    struct List *next;
} List;

void freeList(List *l) {
    if (l->next != NULL) {
        freeList(l->next);
    }

    free(l);
}

List* newListItem(int v) {
    List *l = (List*)malloc(1 * sizeof(List));
    
    l->value = v;
    l->next = NULL;
    return l;
}

void push(List *l, int v) {
    if (l->next != NULL) {
        push(l->next, v);
        return;
    }

    List *i = newListItem(v);
    l->next = i;
}

int kadane(List *list) {
    int curr_sum = 0;
    int max_sum = INT_MIN;
    List *i = list;
    // Itera sobre a lista 
    while(i != NULL) { 
  
        // Adiciona o valor atual a soma 
        curr_sum += i->value;
  
        // Atualiza a soma máxima 
        if (curr_sum > max_sum) {
            max_sum = curr_sum;
        }
  
        if (curr_sum < 0) {
            curr_sum = 0;
        }
        i = i->next;
    }
  
    return max_sum;
}

int maxSubMatrixSum(int *matrix, int matrix_size) {
    int row = matrix_size;
    int column = matrix_size;

    int prefix[row][column];

    // Inicializando uma nova matrix com 0 
    for (int r = 0; r < row; r++) {
        for (int c = 0; c < column; c++) {
            prefix[r][c] = 0;
        }
    }
    
    // Calcula as somas dos items da mesma linha e salva na nova matrix
    for (int r = 0; r < row; r++) { 
        for (int c = 0; c < column; c++) { 
            // Update the prefix[][] 
            if (c == 0) {
                prefix[r][c] = matrix[c + (r * row)];
            } else {
                prefix[r][c] = matrix[c + (r * row)] + prefix[r][c - 1];
            }
        } 
    }

    int max_sum = INT_MIN;
    // Itera sobre a coluna inicial da matrix prefix
    for (int initial_column = 0; initial_column < column; initial_column++) {
        // Itera sobre as colunas com o offset da coluna inicial da matrix prefix 
        for (int c = initial_column; c < column; c++) {
            List *v = NULL;
            // Itera por todas as linhas da matrix prefix
            for (int r = 0; r < row; r++) { 
                int el = 0;
  
                if (initial_column == 0) {
                    el = prefix[r][c];
                } else {
                    el = prefix[r][c] - prefix[r][initial_column - 1];
                }

                if (v == NULL) {
                    v = newListItem(el);
                } else {
                    push(v, el);
                }
            }
            // Atualiza o valor de soma máxima com o valor mais alto,
            // sendo o valor atual ou o retornado pela função kadane
            max_sum = max(max_sum, kadane(v));
            freeList(v);
        } 
    }
    return max_sum;
}
