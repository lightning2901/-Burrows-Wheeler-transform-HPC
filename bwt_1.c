#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>  // Para paralelización


char* read_file(const char* filename, int* file_size) {
    FILE* file = fopen(filename, "rb");  // modo binario para evitar problemas con saltos de línea
    if (!file) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    // Obtener tamaño del archivo
    fseek(file, 0, SEEK_END);
    *file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    
    char* content = (char*)malloc(*file_size + 1); //reserva de memoria del tamaño del archivo file_size
    fread(content, 1, *file_size, file);
    content[*file_size] = '\0';  // Asegurar terminación nula

    fclose(file);
    return content; 
}


char* generate_rotation(const char* text, int n, int rotation_index) { // de la matriz rotación específica
    char* rotation = (char*)malloc((n + 1) * sizeof(char));
    for (int i = 0; i < n; i++) {
        rotation[i] = text[(rotation_index + i) % n];
    }
    rotation[n] = '\0';
    return rotation;
}


int compare_strings(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

// BWT paralelizada
char* bwt_parallel(const char* text, int n) {
    char** rotations = (char**)malloc(n * sizeof(char*));

    // PASO 1: Generar rotaciones (paralelizado)
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        rotations[i] = generate_rotation(text, n, i);
    }

    // PASO 2: Ordenar rotaciones (paralelizado)
    #pragma omp parallel
    {
        #pragma omp single
        qsort(rotations, n, sizeof(char*), compare_strings);
    }

    // PASO 3: Extraer última columna
    char* result = (char*)malloc((n + 1) * sizeof(char));
    for (int i = 0; i < n; i++) {
        result[i] = rotations[i][n - 1];
    }
    result[n] = '\0';

    // Liberar memoria
    for (int i = 0; i < n; i++) {
        free(rotations[i]);
    }
    free(rotations);

    return result;
}

// Programa principal
int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Uso: %s <archivo.txt>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Leer archivo
    int file_size;
    char* text = read_file(argv[1], &file_size);
    printf("Contenido del archivo (%d bytes):\n%s\n", file_size, text);

    // Aplicar BWT
    char* bwt_result = bwt_parallel(text, file_size);
    printf("\nBWT resultante (%d bytes):\n%s\n", file_size, bwt_result);

    // Guardar resultado en un nuevo archivo (opcional)
    FILE* output = fopen("bwt_result.txt", "w");
    fwrite(bwt_result, 1, file_size, output);
    fclose(output);
    printf("\nResultado guardado en 'bwt_result.txt'\n");

    // Liberar memoria
    free(text);
    free(bwt_result);

    return EXIT_SUCCESS;
}
