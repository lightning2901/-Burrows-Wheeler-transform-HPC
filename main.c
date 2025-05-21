#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>  // Para medir tiempo

// ======================================
// Leer archivo completo en memoria
// ======================================
char* read_file(const char* filename, int* file_size) {
    FILE* file = fopen(filename, "rb");  // modo binario
    if (!file) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    *file_size = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(*file_size + 2);  // +2 por seguridad
    if (!buffer) {
        perror("No se pudo asignar memoria");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fread(buffer, 1, *file_size, file);
    buffer[*file_size] = '\0';

    fclose(file);
    return buffer;
}

// ======================================
// Comparador para ordenar sufijos (qsort_r)
// ======================================
int suffix_compare(const void* a, const void* b, void* data) {
    const char* text = (const char*)data;
    int i = *(const int*)a;
    int j = *(const int*)b;

    while (text[i] && text[j]) {
        if (text[i] != text[j])
            return (text[i] - text[j]);
        i++;
        j++;
    }

    return (text[i] - text[j]);
}

// ======================================
// Transformación Burrows-Wheeler optimizada
// ======================================
void burrows_wheeler_transform(const char* input, char* output) {
    int len = strlen(input);
    int* suffix_array = (int*)malloc(len * sizeof(int));
    if (!suffix_array) {
        perror("No se pudo asignar memoria para el suffix array");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < len; i++) {
        suffix_array[i] = i;
    }

    // Ordenar los índices como sufijos
#if defined(__linux__) || defined(__APPLE__)
    qsort_r(suffix_array, len, sizeof(int), suffix_compare, (void*)input);
#else
    fprintf(stderr, "Este compilador no soporta qsort_r. Reemplaza qsort_r manualmente.\n");
    exit(EXIT_FAILURE);
#endif

    for (int i = 0; i < len; i++) {
        int idx = suffix_array[i];
        output[i] = (idx == 0) ? input[len - 1] : input[idx - 1];
    }
    output[len] = '\0';
    free(suffix_array);
}

// ======================================
// Función principal
// ======================================
int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <archivo_entrada.txt> <archivo_salida.txt>\n", argv[0]);
        return 1;
    }

    int file_size;
    char* input = read_file(argv[1], &file_size);

    // Agregar '$' al final si no está presente
    if (input[file_size - 1] != '$') {
        input[file_size] = '$';
        input[file_size + 1] = '\0';
        file_size++;
    }

    char* output = (char*)malloc(file_size + 1);
    if (!output) {
        perror("No se pudo asignar memoria para la salida");
        free(input);
        return 1;
    }

    // Medición de tiempo de ejecución
    clock_t start_time = clock();

    burrows_wheeler_transform(input, output);

    clock_t end_time = clock();
    double elapsed = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Transformación BWT completada en %.4f segundos.\n", elapsed);

    FILE* output_file = fopen(argv[2], "w");
    if (!output_file) {
        perror("No se pudo abrir el archivo de salida");
        free(input);
        free(output);
        return 1;
    }

    fprintf(output_file, "%s\n", output);
    fclose(output_file);

    printf("Resultado escrito en: %s\n", argv[2]);

    free(input);
    free(output);
    return 0;
}
