#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>  // Para OpenMP

// Lee todo el contenido de un archivo en memoria
char* read_file(const char* filename, int* file_size) {
    FILE* file = fopen(filename, "rb"); // Abrir en binario
    if (!file) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    *file_size = ftell(file); // Tamaño del archivo
    fseek(file, 0, SEEK_SET);

    char* content = (char*)malloc(*file_size + 2); '
    fread(content, 1, *file_size, file);
    fclose(file);

    content[*file_size] = '\1';      // Agrega símbolo de fin de texto
    content[*file_size + 1] = '\0';  // Final de cadena

    *file_size += 1; 
    return content;
}

// Compara sufijos a partir de sus posiciones
int suffix_compare(const void* a, const void* b, void* arg) {
    const char* text = (const char*)arg;
    int i = *(const int*)a;
    int j = *(const int*)b;
    return strcmp(text + i, text + j);
}

// Construye el suffix array (secuencial)
int* build_suffix_array(const char* text, int n) {
    int* suffix_array = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        suffix_array[i] = i;

    // Ordenar sufijos usando comparación personalizada (secuencial)
    qsort_r(suffix_array, n, sizeof(int), suffix_compare, (void*)text);
    return suffix_array;
}

// Aplica la transformación de Burrows-Wheeler (PARALELIZADA con OpenMP)
char* burrows_wheeler_transform(const char* text, int* suffix_array, int n) {
    char* bwt = (char*)malloc((n + 1) * sizeof(char));
    
    #pragma omp parallel for  // Paralelización del bucle
    for (int i = 0; i < n; i++) {
        int idx = suffix_array[i];
        bwt[i] = (idx == 0) ? text[n - 1] : text[idx - 1];
    }
    bwt[n] = '\0';
    return bwt;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Uso: %s <archivo_entrada> <archivo_salida>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Leer archivo
    int file_size;
    char* text = read_file(argv[1], &file_size);
    printf("Tamaño leído: %d bytes\n", file_size);

    // Construir suffix array (secuencial)
    double start_time = omp_get_wtime();
    int* suffix_array = build_suffix_array(text, file_size);
    double build_time = omp_get_wtime() - start_time;
    printf("Suffix array construido en %.4f segundos\n", build_time);

    // Aplicar BWT (paralelizado)
    start_time = omp_get_wtime();
    char* bwt_result = burrows_wheeler_transform(text, suffix_array, file_size);
    double bwt_time = omp_get_wtime() - start_time;
    printf("BWT completada en %.4f segundos (paralelizado con OpenMP)\n", bwt_time);

    // Guardar en archivo de salida
    FILE* output = fopen(argv[2], "w");
    if (!output) {
        perror("Error al escribir archivo de salida");
        return EXIT_FAILURE;
    }
    fwrite(bwt_result, 1, file_size, output);
    fclose(output);
    printf("Resultado guardado en '%s'\n", argv[2]);

    // Liberar memoria
    free(text);
    free(suffix_array);
    free(bwt_result);

    return EXIT_SUCCESS;
}
