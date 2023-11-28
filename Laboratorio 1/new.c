//-------------------LIBRERIAS-------------------//
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

//---------------DEFINE AND GLOBAL---------------//
#define MAX_RUTA 512
char directorioOriginal[256];

//-------------------FUNCIONES-------------------//
void buscar_palabra(const char *palabra, const char *linea, int num, const char *tipo, int *encontrada) {
    /*
    BUSCA LA PALABRA A ENCONTRAR EN UNA LINEA DE TEXTO, SI LA PALABRA ESTA EN LA LINEA SE 
    MARCA
    */
    if (strstr(linea, palabra) != NULL) {
        *encontrada = 1; // Marcar que la palabra fue encontrada
    } 
}

void horizontal(const char *nombreArchivo, const char *palabra) {
    /*
    PROCESA EL ARCHIVO DE TEXTO DE LA PALABRA A ENCONTRAR, ESPECIFICAMENTE LAS
    QUE TENGAN ORIENTACION HORIZONTAL. DE ESTA FUNCION EL TIEMPO COMIENZA A PENAS
    EL ARCHIVO ES ABIERTO Y TERMINA CUANDO LA PALABRA ES ENCONTRADA.
    */
    FILE *file;
    char line[500]; 
    int count = 0;
    int encontrada = 0;

    file = fopen(nombreArchivo, "r");

    clock_t start_time = clock(); 

    if (file == NULL) {
        printf("No se pudo abrir el archivo %s.\n", nombreArchivo);
        exit(1);
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        count++;
        if (count == 1) {
            continue;
        }

        int j = 0;
        for (int i = 0; line[i]; i++) {
            if (!isspace(line[i])) {
                line[j++] = tolower(line[i]);
            }
        }
        line[j] = '\0';

        buscar_palabra(palabra, line, count, "línea",&encontrada);

        if (encontrada) {
            break;
        }
    }

    clock_t end_time = clock(); // Finalizar el contador de tiempo
    double segundos = (double)(end_time - start_time) / CLOCKS_PER_SEC; // Calcular el tiempo en segundos

    if (encontrada) {
        printf("→ Tiempo para encontrar la palabra: %f segundos\n\n", segundos);
    } else {
        printf("La palabra no fue encontrada.\n");
    }

    fclose(file);
}

void vertical(const char *nombreArchivo, const char *palabra) {
     /*
    PROCESA EL ARCHIVO DE TEXTO DE LA PALABRA A ENCONTRAR, ESPECIFICAMENTE LAS
    QUE TENGAN ORIENTACION VERTICAL. DE ESTA FUNCION EL TIEMPO COMIENZA A PENAS
    EL ARCHIVO ES ABIERTO Y TERMINA CUANDO LA PALABRA ES ENCONTRADA.
    */
    FILE *file;
    char matrix[500][500];
    int rows = 0, cols = 0;
    int encontrada = 0;

    file = fopen(nombreArchivo, "r");
    clock_t start_time = clock(); 

    if (file == NULL) {
        printf("No se pudo abrir el archivo %s.\n", nombreArchivo);
        exit(1);
    }

    char ignoreFirstLine[500];
    fgets(ignoreFirstLine, sizeof(ignoreFirstLine), file);

    while (fgets(matrix[rows], sizeof(matrix[rows]), file) != NULL) {
        if (rows == 0) {
            cols = strlen(matrix[rows]);
        }
        rows++;
    }

    fclose(file);

    for (int j = 0; j < cols; j++) {
        char column[500];
        int k = 0;
        for (int i = 0; i < rows; i++) {
            if (!isspace(matrix[i][j])) {
                column[k++] = tolower(matrix[i][j]);
            }
        }
        column[k] = '\0';

        buscar_palabra(palabra, column, j+1, "columna",&encontrada);

        if (encontrada) {
            break;
        }
    }

    clock_t end_time = clock(); // Finalizar el contador de tiempo
    double segundos = (double)(end_time - start_time) / CLOCKS_PER_SEC; // Calcular el tiempo en segundos

    if (encontrada) {
        printf("→ Tiempo para encontrar la palabra: %f segundos\n\n", segundos);
    } else {
        printf("La palabra no fue encontrada.\n");
    }
}

void QuitarExtension(char* nombre) {
    /*
    ELIMINA LA EXTENSION .TXT DE ALGUNA PALABRA Y CONVIERTE TODOS
    LOS CARACTERES A MINUSCULAS
    */
    char* punto = strrchr(nombre, '.');
    if (punto != NULL) {
        *punto = '\0';
    }

    for (int i = 0; nombre[i]; i++) {
        nombre[i] = tolower((unsigned char)nombre[i]);
    }
}

int contarLineasArchivo(const char *nombreArchivo, char *primeraLinea) {
     /*
    BUSCA EL TAMANO DE LA MATRIZ PARA SABER SI ES: 50X50, 100X100 O 200X200
    */
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        perror("Error abriendo el archivo");
        return -1;
    }

    int lineas = 0;
    char buffer[512]; 

    while (fgets(buffer, sizeof(buffer), archivo) != NULL) {
        lineas++;
        if (lineas == 1) {
            strcpy(primeraLinea, buffer);
        }
    }

    fclose(archivo);
    return lineas;
}

void crearSubcarpetas( char *nombreCarpeta) {
    /*
    CREA LAS SUBCARPETAS 50X50, 100X100 Y 200X200 DENTRO DE LAS 
    CARPETAS HORIZONTAL Y VERTICAL.
    */

    if (chdir(nombreCarpeta) == -1) {
        perror("Error cambiando al directorio");
        exit(1);
    }

    // Crear las carpetas 50x50, 100x100 y 200x200
    if (mkdir("50x50", 0777) == -1) {
        perror("Error creando la carpeta 50x50");
        exit(1);
    }
    if (mkdir("100x100", 0777) == -1) {
        perror("Error creando la carpeta 100x100");
        exit(1);
    }
    if (mkdir("200x200", 0777) == -1) {
        perror("Error creando la carpeta 200x200");
        exit(1);
    }

    if (chdir(directorioOriginal) == -1) { 
        perror("Error regresando al directorio padre");
        exit(1);
    }
}

void Carpetas() {
    /*
    CREA LA CARPETA CWD Y DENTRO DE ELLA CREA A HORZIONTAL Y
    VERTICAL, LUEGO LLAMA A LA FUNCION 'crearSubCarpetas'
    */

   // Almacenar el directorio original
    if (getcwd(directorioOriginal, sizeof(directorioOriginal)) == NULL) {
        perror("Error obteniendo el directorio actual");
        exit(1);
    }

    // Crear la carpeta CWD
    if (mkdir("CWD", 0777) == -1) {
        perror("Error creando la carpeta CWD");
        exit(1);
    }

    if (chdir("CWD") == -1) {
        perror("Error cambiando al directorio CWD");
        exit(1);
    }

    // Crear carpetas Horizontal y Vertical
    if (mkdir("Horizontal", 0777) == -1) {
        perror("Error creando la carpeta Horizontal");
        exit(1);
    }

    crearSubcarpetas("Horizontal");

    if (chdir(directorioOriginal) == -1) { 
        perror("Error regresando al directorio padre");
        exit(1);
    }

    if (chdir("CWD") == -1) {
        perror("Error cambiando al directorio CWD");
        exit(1);
    }

    if (mkdir("Vertical", 0777) == -1) {
        perror("Error creando la carpeta Vertical");
        exit(1);
    }

    crearSubcarpetas("Vertical");   
}

//---------------------MAIN---------------------//
int main() {
    Carpetas();

    DIR *directorio = opendir(".");
    if (directorio == NULL) {
        perror("Error abriendo el directorio");
        return 1;
    }

    struct dirent *entrada;

    while ((entrada = readdir(directorio)) != NULL) {
        if (entrada->d_type == DT_REG && strstr(entrada->d_name, ".txt")) {

            char nombre[256];
            strcpy(nombre, entrada->d_name);
            QuitarExtension(nombre);

            char rutaArchivo[256];
            int red = snprintf(rutaArchivo, sizeof(rutaArchivo), "./%s", entrada->d_name);
            if (red < 0) {
                perror("Valor negativo del snprintf");
                return 1;
            }

            char primeraLinea[MAX_RUTA];
            int lineas = contarLineasArchivo(rutaArchivo, primeraLinea);

            if (lineas >= 0) {
                char destino[MAX_RUTA];
                
                if (strstr(primeraLinea, "vertical")) {
                    printf("Archivo: %s | Matriz: %d x %d | Orientacion: %s", entrada->d_name, lineas-1, lineas-1, primeraLinea);
                    vertical(entrada->d_name, nombre);

                    if (lineas - 1 == 50) {
                        snprintf(destino, sizeof(destino), "CWD/Vertical/50x50/%s", entrada->d_name);
                        if (rename(rutaArchivo, destino) != 0) {
                            perror("Error moviendo archivo");
                        }

                    } else if (lineas - 1 == 100) {
                        snprintf(destino, sizeof(destino), "CWD/Vertical/100x100/%s", entrada->d_name);
                        if (rename(rutaArchivo, destino) != 0) {
                            perror("Error moviendo archivo");
                        }

                    } else if (lineas - 1 == 200) {
                        snprintf(destino, sizeof(destino), "CWD/Vertical/200x200/%s", entrada->d_name);
                        if (rename(rutaArchivo, destino) != 0) {
                            perror("Error moviendo archivo");
                        }

                    }
                } else if (strstr(primeraLinea, "horizontal")) {
                    printf("Archivo: %s | Matriz: %d x %d | Orientacion: %s", entrada->d_name, lineas-1, lineas-1, primeraLinea);
                    horizontal(entrada->d_name, nombre);

                    if (lineas - 1 == 50) {
                        snprintf(destino, sizeof(destino), "CWD/Horizontal/50x50/%s", entrada->d_name);
                        if (rename(rutaArchivo, destino) != 0) {
                            perror("Error moviendo archivo");
                        }

                    } else if (lineas - 1 == 100) {
                        snprintf(destino, sizeof(destino), "CWD/Horizontal/100x100/%s", entrada->d_name);
                        if (rename(rutaArchivo, destino) != 0) {
                            perror("Error moviendo archivo");
                        }

                    } else if (lineas - 1 == 200) {
                        snprintf(destino, sizeof(destino), "CWD/Horizontal/200x200/%s", entrada->d_name);
                        if (rename(rutaArchivo, destino) != 0) {
                            perror("Error moviendo archivo");
                        }

                    }
                }
            }
        }
    }

    closedir(directorio);
    return 0;
}