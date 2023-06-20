#include<stdio.h>
#include <stdlib.h>
#include <time.h>

int validarArquivo(FILE *arquivo, char tipo) {
	if(arquivo == NULL) {
		printf("erro ao abrir o arquivo");
		return 1;	
	} 
	
	if(tipo == 'r') {
		if (fgetc(arquivo) == EOF) {
	        printf("Arquivo vazio.\n");
	        fclose(arquivo);
	        return 1;
	    }
	    rewind(arquivo);
	}
	
    return 0;
}

int validarTamanhoVetor(FILE* arquivo, int* tam) {
    if (fscanf(arquivo, "%d", tam) != 1) {
        printf("Erro ao ler o tamanho do vetor.\n");
        fclose(arquivo);
        return 1;
    }

    return 0;
}

int validarMetodoOrdenacao(FILE* arquivo, char* metodo) {
    if (fscanf(arquivo, " %c", metodo) != 1 ||
        (*metodo != 'c' && *metodo != 'd' && *metodo != 'r')) {
        printf("Erro ao ler o metodo de ordenacao.\n");
        fclose(arquivo);
        return 1;
    }

    return 0;
}

int* alocarVetor(int tam, FILE* arquivo) {
    int* vetor = (int*)malloc(tam * sizeof(int));
    if (vetor == NULL) {
        printf("Erro ao alocar memoria para o vetor.\n");
        fclose(arquivo);
        return NULL;
    }

    return vetor;
}

void preencherVetor(int vetor[], int tam, char metodo) {
    int i;

    if (metodo == 'c') {
        for (i = 0; i < tam; i++) {
            vetor[i] = i + 1;
        }
    } else if (metodo == 'd') {
        for (i = 0; i < tam; i++) {
            vetor[i] = tam - i;
        }
    } else if (metodo == 'r') {
        srand(time(NULL));
        for (i = 0; i < tam; i++) {
            vetor[i] = rand() % 32000;
        }
    }
}

void imprimirVetor(int vetor[], int tamanho, FILE* arquivoSaida) {
    int i;

    for (i = 0; i < tamanho; i++) {
        fprintf(arquivoSaida, "%d ", vetor[i]);
    }
}

void insertionSort(int vetor[], int tamanho, FILE* arquivoSaida) {
    int i, chave, j, comparacoes = 0;
    clock_t start, end;
    double diff;
    
    start = clock();
    
    for (i = 1; i < tamanho; i++) {
        chave = vetor[i];
        j = i - 1;

        comparacoes++;
        while (j >= 0 && vetor[j] > chave) {
            vetor[j + 1] = vetor[j];
            j = j - 1;
            comparacoes++;
        }
        vetor[j + 1] = chave;
    }

    end = clock();
    diff = ((double)(end - start) / CLOCKS_PER_SEC) * 1000 ;

    fprintf(arquivoSaida, "insertionSort ");
    imprimirVetor(vetor, tamanho, arquivoSaida);
    fprintf(arquivoSaida, "Comparacoes: %d ", comparacoes);
    fprintf(arquivoSaida, "Tempo: %f ms", diff);
}

void selectionSort(int vetor[], int tamanho, FILE* arquivoSaida) {
    int i, j, min_idx, comparacoes = 0;
    clock_t start, end;
    double diff;

    start = clock();

    for (i = 0; i < tamanho - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < tamanho; j++) {
            comparacoes++;
            if (vetor[j] < vetor[min_idx]) {
                min_idx = j;
            }
        }
        int temp = vetor[min_idx];
        vetor[min_idx] = vetor[i];
        vetor[i] = temp;
    }

    end = clock();
    diff = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;

    fprintf(arquivoSaida, "selectionSort ");
    imprimirVetor(vetor, tamanho, arquivoSaida);
    fprintf(arquivoSaida, "Comparacoes: %d ", comparacoes);
    fprintf(arquivoSaida, "Tempo: %f ms", diff);
}

void bubbleSort(int vetor[], int tamanho, FILE* arquivoSaida) {
    int i, j, comparacoes = 0;
    clock_t start, end;
    double diff;

    start = clock();

    for (i = 0; i < tamanho - 1; i++) {
        for (j = 0; j < tamanho - i - 1; j++) {
            comparacoes++;
            if (vetor[j] > vetor[j + 1]) {
                int temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
            }
        }
    }

    end = clock();
    diff = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;

    fprintf(arquivoSaida, "bubbleSort ");
    imprimirVetor(vetor, tamanho, arquivoSaida);
    fprintf(arquivoSaida, "Comparacoes: %d ", comparacoes);
    fprintf(arquivoSaida, "Tempo: %f ms", diff);
}

// Função auxiliar para o merge sort
void merge(int vetor[], int inicio, int meio, int fim, int *comparacoes) {
    int i, j, k;
    int n1 = meio - inicio + 1;
    int n2 = fim - meio;
    int L[n1], R[n2];

    for (i = 0; i < n1; i++) {
        L[i] = vetor[inicio + i];
    }
    for (j = 0; j < n2; j++) {
        R[j] = vetor[meio + 1 + j];
    }

    i = 0;
    j = 0;
    k = inicio;

    while (i < n1 && j < n2) {
        (*comparacoes)++;
        if (L[i] <= R[j]) {
            vetor[k] = L[i];
            i++;
        } else {
            vetor[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        vetor[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        vetor[k] = R[j];
        j++;
        k++;
    }
}

// Função de ordenação Merge Sort
void mergeSort(int vetor[], int inicio, int fim, int *comparacoes) {
    if (inicio < fim) {
        int meio = inicio + (fim - inicio) / 2;

        mergeSort(vetor, inicio, meio, comparacoes);
        mergeSort(vetor, meio + 1, fim, comparacoes);

        merge(vetor, inicio, meio, fim, comparacoes);
    }
}

// Função wrapper para o Merge Sort
void wrapperMergeSort(int vetor[], int tamanho, FILE* arquivoSaida) {
    int comparacoes = 0;
    int *copia_vetor = malloc(tamanho * sizeof(int));
    clock_t start, end;
    double diff;

    start = clock();

    memcpy(copia_vetor, vetor, tamanho * sizeof(int));
    mergeSort(copia_vetor, 0, tamanho - 1, &comparacoes);

    end = clock();
    diff = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;

    fprintf(arquivoSaida, "mergeSort ");
    imprimirVetor(copia_vetor, tamanho, arquivoSaida);
    fprintf(arquivoSaida,"Comparacoes: %d ", comparacoes);
    fprintf(arquivoSaida, "Tempo: %f ms", diff);

    free(copia_vetor);
}

// Função auxiliar para o quick sort
int particionar(int vetor[], int inicio, int fim, int *comparacoes) {
    int pivo = vetor[fim];
    int i = (inicio - 1);
    int j, temp;

    for (j = inicio; j <= fim - 1; j++) {
        (*comparacoes)++;
        if (vetor[j] < pivo) {
            i++;
            temp = vetor[i];
            vetor[i] = vetor[j];
            vetor[j] = temp;
        }
    }
    temp = vetor[i + 1];
    vetor[i + 1] = vetor[fim];
    vetor[fim] = temp;

    return (i + 1);
}

// Função de ordenação Quick Sort
void quickSort(int vetor[], int inicio, int fim, int *comparacoes) {
    if (inicio < fim) {
        int pivo = particionar(vetor, inicio, fim, comparacoes);

        quickSort(vetor, inicio, pivo - 1, comparacoes);
        quickSort(vetor, pivo + 1, fim, comparacoes);
    }
}

// Função wrapper para o Quick Sort
void wrapperQuickSort(int vetor[], int tamanho, FILE* arquivoSaida) {
    int comparacoes = 0;
    int *copia_vetor = malloc(tamanho * sizeof(int));
    clock_t start, end;
    double diff;

    start = clock();

    memcpy(copia_vetor, vetor, tamanho * sizeof(int));
    quickSort(copia_vetor, 0, tamanho - 1, &comparacoes);

    end = clock();
    diff = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;

    fprintf(arquivoSaida,"quickSort ");
    imprimirVetor(copia_vetor, tamanho, arquivoSaida);
    fprintf(arquivoSaida,"Comparacoes: %d ", comparacoes);
    fprintf(arquivoSaida,"Tempo: %f ms", diff);

    free(copia_vetor);
}

// Função auxiliar para o heapify
void heapify(int vetor[], int tamanho, int indice, int *comparacoes) {
    int maior = indice;
    int esquerda = 2 * indice + 1;
    int direita = 2 * indice + 2;
    int temp;

    if (esquerda < tamanho && vetor[esquerda] > vetor[maior]) {
        (*comparacoes)++;
        maior = esquerda;
    }

    if (direita < tamanho && vetor[direita] > vetor[maior]) {
        (*comparacoes)++;
        maior = direita;
    }

    if (maior != indice) {
        temp = vetor[indice];
        vetor[indice] = vetor[maior];
        vetor[maior] = temp;

        heapify(vetor, tamanho, maior, comparacoes);
    }
}

// Função de ordenação Heap Sort
void heapSort(int vetor[], int tamanho, FILE* arquivoSaida) {
    int comparacoes = 0;
    int *copia_vetor = malloc(tamanho * sizeof(int));
    clock_t start, end;
    double diff;

    start = clock();

    memcpy(copia_vetor, vetor, tamanho * sizeof(int));

    int i;
    for (i = tamanho / 2 - 1; i >= 0; i--) {
        heapify(copia_vetor, tamanho, i, &comparacoes);
    }

    for (i = tamanho - 1; i >= 0; i--) {
        int temp = copia_vetor[0];
        copia_vetor[0] = copia_vetor[i];
        copia_vetor[i] = temp;

        heapify(copia_vetor, i, 0, &comparacoes);
    }

    end = clock();
    diff = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;

    fprintf(arquivoSaida,"heapSort ");
    imprimirVetor(copia_vetor, tamanho, arquivoSaida);
    fprintf(arquivoSaida, "Comparacoes: %d ", comparacoes);
    fprintf(arquivoSaida,"Tempo: %f ms", diff);

    free(copia_vetor);
}

int main(int argc, char* argv[]) {
	
	FILE *arquivoEntrada = fopen(argv[1], "r");
	if(validarArquivo(arquivoEntrada, 'r') != 0) return 1; 
	int tam;
	if(validarTamanhoVetor(arquivoEntrada, &tam) != 0) return 1;
	char metodo;
	if(validarMetodoOrdenacao(arquivoEntrada, &metodo) != 0) return 1;
	
	int vetor[tam];
    preencherVetor(vetor, tam, metodo);
	
	FILE *arquivoSaida = fopen(argv[2], "w");
	if(validarArquivo(arquivoSaida, 'w') != 0) {
		free(vetor);
		return 1;
	} 
	
	if(metodo == 'c') {
		fprintf(arquivoSaida, "Metodo crescente\n");
	} else if(metodo == 'd') {
		fprintf(arquivoSaida, "Metodo decrescente\n");
	} else if (metodo == 'r') {
		fprintf(arquivoSaida, "Metodo randomico\n");
	}
	
	int *copia_vetor = malloc(tam * sizeof(int));
	
	memcpy(copia_vetor, vetor, tam * sizeof(int));
	insertionSort(copia_vetor, tam, arquivoSaida);
    fprintf(arquivoSaida, "\n");
    
    memcpy(copia_vetor, vetor, tam * sizeof(int));
    selectionSort(copia_vetor, tam, arquivoSaida);
    fprintf(arquivoSaida, "\n");
    
    memcpy(copia_vetor, vetor, tam * sizeof(int));
	bubbleSort(copia_vetor, tam, arquivoSaida);
	fprintf(arquivoSaida, "\n");
	
	memcpy(copia_vetor, vetor, tam * sizeof(int));
	wrapperMergeSort(copia_vetor, tam, arquivoSaida);
	fprintf(arquivoSaida, "\n");
	
	memcpy(copia_vetor, vetor, tam * sizeof(int));
	wrapperQuickSort(copia_vetor, tam, arquivoSaida);
	fprintf(arquivoSaida, "\n");
	
	memcpy(copia_vetor, vetor, tam * sizeof(int));
	heapSort(copia_vetor, tam, arquivoSaida);
	fprintf(arquivoSaida, "\n");
	
	free(copia_vetor);
	fclose(arquivoEntrada);
	fclose(arquivoSaida);
	return 0;
}

