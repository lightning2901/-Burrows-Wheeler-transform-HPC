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

    fseek(file, 0, SEEK_END); //inicio del file
    *file_size = ftell(file); // tamaño en sí del archivo
    fseek(file, 0, SEEK_SET); //end del file

    char* content = (char*)malloc(*file_size + 2); '
      fread(content, 1, *file_size, file); //fread es una función para leer todo el archivo de manera binaria para no suponer cosas con el tipo de dato
    fclose(file);

    content[*file_size] = '\1';      // agregar símbolo de fin de texto
    content[*file_size + 1] = '\0';  // final de cadena, importante para los sufijos posteriormente con strcmp 

    *file_size += 1; 
    return content; //contenido del txt 
}

// Compara sufijos a partir de sus posiciones
// para esto uso strcmp,strcmp() compara string1 y string2. La función opera en series con final nulo. Los argumentos de serie para la función deben contener un carácter nulo (\0) que marque el final de la serie.
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
        suffix_array[i] = i;  // Inicializa con índices 0, 1, 2, ..., n-1

    // Ordenar sufijos usando comparación personalizada (secuencial)
    qsort_r(suffix_array, n, sizeof(int), suffix_compare, (void*)text);
    return suffix_array;// The comparison function must return an integer less than, equal to, or greater than zero if the first argument is considered to be respectively less than, equal to, or greater than the second. If two members compare as equal, their order in the sorted array is undefined.
}

// bwt con OpenMP
char* burrows_wheeler_transform(const char* text, int* suffix_array, int n) {
    char* bwt = (char*)malloc((n + 1) * sizeof(char));
    
    #pragma omp parallel for  // Paralelización del bucle empezando con pragma, la bandera para que se asigne la tarea a los nucleos
    for (int i = 0; i < n; i++) {  //this directive will cause slave threads to form. Each thread executes the parallel section of the code independently. When a thread finishes, it joins the master. When all threads finish, the master continues with code following the parallel section.
      //así que a diferencia de mpi no requerimos nosotros crear o tener la comunicación de los procesos de manera manual, existen ID's en los hilos pero no una declaración implícita, porque es una sola cpu con una asignación de memoria igual para todos. 
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
