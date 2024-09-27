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
    // Stores current and maximum sum 
    int curr_sum = 0;
    int max_sum = INT_MIN;
    List *i = list;
    // Traverse the array v 
    while(i != NULL) { 
  
        // Add the value of the 
        // current element 
        curr_sum += i->value;
  
        // Update the maximum sum 
        if (curr_sum > max_sum) {
            max_sum = curr_sum;
        }
  
        if (curr_sum < 0) {
            curr_sum = 0;
        }
        i = i->next;
    }
  
    // Return the maximum sum 
    return max_sum;
}

int maxSubMatrixSum(int *matrix, int matrix_size) {
    int row = matrix_size;
    int column = matrix_size;

    int prefix[row][column];

    // Traverse the matrix, prefix 
    // and initialize it will all 0s 
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            prefix[i][j] = 0;
        }
    }
    
    // Calculate prefix sum of all 
    // rows of matrix A[][] and 
    // store in matrix prefix[] 
    for (int i = 0; i < row; i++) { 
        for (int j = 0; j < column; j++) { 
            // Update the prefix[][] 
            if (j == 0) {
                prefix[i][j] = matrix[j + (i * row)];
            } else {
                prefix[i][j] = matrix[j + (i * row)] + prefix[i][j - 1];
            }
        } 
    }

    int max_sum = INT_MIN;
    // Iterate for starting column 
    for (int i = 0; i < column; i++) {
        // Iterate for last column 
        for (int j = i; j < column; j++) {
            // To store current array 
            // elements 
            List *v = NULL;
            // Traverse every row 
            for (int k = 0; k < row; k++) {
  
                // Store the sum of the 
                // kth row 
                int el = 0;
  
                // Update the prefix 
                // sum 
                if (i == 0) {
                    el = prefix[k][j];
                } else {
                    el = prefix[k][j] - prefix[k][i - 1];
                }

                // Push it in a vector 
                if (v == NULL) {
                    v = newListItem(el);
                } else {
                    push(v, el);
                }
            }
            // Update the maximum 
            // overall sum 
            max_sum = max(max_sum, kadane(v));
            freeList(v);
        } 
    }
    return max_sum;
}
