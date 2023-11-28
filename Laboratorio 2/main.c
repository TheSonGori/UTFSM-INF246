/* L I B R A R Y */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>

/* D E F I N E */
#define BOARD_SIZE 85
#define FILAS_PEQUENA 5
#define COLUMNAS_PEQUENA 5
#define FILAS_GRANDE 85
#define COLUMNAS_GRANDE 85

/* G L O B A L */
char Mapa[FILAS_GRANDE][COLUMNAS_GRANDE][3];
char Inicio[FILAS_PEQUENA][COLUMNAS_PEQUENA][3];
int opciones_adicionales[4];
bool Flags[8] = {false,false,false,false,false,false,false,false};
bool Tesoro[4] = {false,false,false,false};

/* F U N C T I O N S */
void mostrarSimbolos() {
    /*
    Muestra los simbolos utilizados en el juego (Simbologia).
    */
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                                                           SIMBOLOS EN EL MAPA                                                                              ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║  1. 1 - Jugador 1                                                                                                                                                          ║\n");
    printf("║  2. 2 - Jugador 2                                                                                                                                                          ║\n");
    printf("║  3. 3 - Jugador 3                                                                                                                                                          ║\n");
    printf("║  4. 4 - Jugador 4                                                                                                                                                          ║\n");
    printf("║  5. C - Camara                                                                                                                                                             ║\n");
    printf("║  6. B - Buscar                                                                                                                                                             ║\n");
    printf("║  7. E - Escalera                                                                                                                                                           ║\n");
    printf("║  8. + - 5 turnos mas                                                                                                                                                       ║\n");
    printf("║  9. - - 3 turnos menos                                                                                                                                                     ║\n");
    printf("║ 10. X - Tesoro para Jugador 1                                                                                                                                              ║\n");
    printf("║ 11. Y - Tesoro para Jugador 2                                                                                                                                              ║\n");
    printf("║ 12. W - Tesoro para Jugador 3                                                                                                                                              ║\n");
    printf("║ 13. Z - Tesoro para Jugador 4                                                                                                                                              ║\n");
    printf("║ 14. T - Teletransportador para los Jugadores                                                                                                                               ║\n");
    printf("║ 15. / - Muros que los jugadores no pueden atravesar                                                                                                                        ║\n");
    printf("║ 16. 0 - Casillas donde se puede mover el jugador                                                                                                                           ║\n");
    printf("║ 17. S - Salida                                                                                                                                                             ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
}

void llenarTPs(char Mapa[FILAS_GRANDE][COLUMNAS_GRANDE][3], int tepes[4][2]) {
    /*
    Busca en el Mapa los 'T' y anota las coordenadas en una matriz de Tps.
    */
    int countT = 0;  // para contar cuántos 'T' hemos encontrado
    for (int i = 0; i < FILAS_GRANDE; i++) {
        for (int j = 0; j < COLUMNAS_GRANDE; j++) {
            if (strcmp(Mapa[i][j], "T") == 0) {
                bool yaRegistrado = false;

                // Verificar si ya fue registrado
                for (int k = 0; k < countT; k++) {
                    if (tepes[k][0] == i && tepes[k][1] == j) {
                        yaRegistrado = true;
                        break;
                    }
                }

                if (!yaRegistrado) {
                    tepes[countT][0] = i;
                    tepes[countT][1] = j;
                    countT++;
                }

                // Si ya encontramos 4 'T', no es necesario seguir buscando.
                if (countT == 4) {
                    return;
                }
            }
        }
    }
}

void mostrarTPS(int tepes[4][2]) {
    /*
    Muestra el contenido de la matriz Tps con sus coordenadas.
    */

    for (int i = 0; i < 4; i++) {
        if (tepes[i][0] != -1 && tepes[i][1] != -1) {  // Solo mostrar entradas válidas
            printf("T%d está en las coordenadas (%d, %d)\n", i + 1, tepes[i][0], tepes[i][1]);
        }
    }
}

void CrearMatriz(char matriz[85][85][3]) {
    /*
    Se crea una matriz llamada Mapa, la cual es el tablero principal del juego.
    */

    for (int i = 0; i < 85; i++) {
        for (int j = 0; j < 85; j++) {
            strcpy(matriz[i][j], " "); 
        }
    }
}

int buscarCamaras(char Mapa[FILAS_GRANDE][COLUMNAS_GRANDE][3]) {
    /*
    Busca las todos los caracteres 'C' que esten en el mapa y retorna y la cantidad de C que encuentre.
    */

    int count = 0; 
    
    for (int i = 0; i < FILAS_GRANDE; i++) {
        for (int j = 0; j < COLUMNAS_GRANDE; j++) {
            if (strcmp(Mapa[i][j], "C") == 0) { 
                count++; 
            }
        }
    }
    
    return count; 
}

void InsertarMatriz(char matrizGrande[FILAS_GRANDE][COLUMNAS_GRANDE][3], char matrizPequena[FILAS_PEQUENA][COLUMNAS_PEQUENA][3]) {
    /*
    Se inserta el tablero inicial (Inicio.txt) en el Mapa. 
    */

    int filaInicio = (FILAS_GRANDE - FILAS_PEQUENA) / 2;
    int columnaInicio = (COLUMNAS_GRANDE - COLUMNAS_PEQUENA) / 2;

    for (int fila = 0; fila < FILAS_PEQUENA; fila++) {
        for (int columna = 0; columna < COLUMNAS_PEQUENA; columna++) {
            strcpy(matrizGrande[filaInicio + fila][columnaInicio + columna], matrizPequena[fila][columna]);
        }
    }
}

void LectorTXT(char *nombreArchivo, char matriz[5][5][3]) {
    /*
    Lee un archivo y los almacena en una matriz 5x5.
    */

    FILE *archivo = fopen(nombreArchivo, "r");
    for (int fila = 0; fila < FILAS_PEQUENA; fila++) {
        for (int columna = 0; columna < COLUMNAS_PEQUENA; columna++) {
            fscanf(archivo, "%2s", matriz[fila][columna]);
        }
    }
    fclose(archivo);
}

void ImprimirMatriz(char matriz[85][85][3]) {
    /*
    Imprime el Mapa.
    */

    printf("╔");
    for (int i = 0; i < 86; i++) {
        printf("══");
    }
    printf("╗\n");

    for (int fila = 0; fila < 85; fila++) {
        printf("║ ");
        for (int columna = 0; columna < 85; columna++) {
            printf("%s ", matriz[fila][columna]);
        }
        printf(" ║\n");
    }

    printf("╚");
    for (int i = 0; i < 86; i++) {
        printf("══");
    }
    printf("╝\n");
}

void CambiarCaracteres(char matriz[FILAS_PEQUENA][COLUMNAS_PEQUENA][3]) {
    /*
    Cambia la simbologia inicial de los jugadores del mapa a:
        J1 - 1
        J2 - 2
        J3 - 3
        J4 - 4
    Esto se hace para que a la hora que se imprima el Mapa este no se vea desfasado.
    */
    
    for (int fila = 0; fila < FILAS_PEQUENA; fila++) {
        for (int columna = 0; columna < COLUMNAS_PEQUENA; columna++) {
            if (strcmp(matriz[fila][columna], "J1") == 0) {
                strcpy(matriz[fila][columna], "1");
            } else if (strcmp(matriz[fila][columna], "J2") == 0) {
                strcpy(matriz[fila][columna], "2");
            } else if (strcmp(matriz[fila][columna], "J3") == 0) {
                strcpy(matriz[fila][columna], "3");
            } else if (strcmp(matriz[fila][columna], "J4") == 0) {
                strcpy(matriz[fila][columna], "4");
            }
        }
    }
}

void InsertarMapas(int filaInicio, int columnaInicio, char matrizPequena[5][5][3], char matrizGrande[85][85][3]){
    /*
    Se inserta algun tablero de la forma en txt de 'tablerox.txt' al Mapa.
    */

    for (int fila = 0; fila < FILAS_PEQUENA; fila++) {
        for (int columna = 0; columna < COLUMNAS_PEQUENA; columna++) {
            strcpy(matrizGrande[filaInicio + fila][columnaInicio + columna], matrizPequena[fila][columna]);
        }
    }
}

void InsertarTesoro(char Mapas[5][5][3]){
    /*
    Se inserta un tesoro en el Mapa. Teniendo un respectivo juego de probabilidades para saber cual se agrega.
    */

    int fila, columna, x;
    do {
        fila = rand() % 5;
        columna = rand() % 5;
    } while (Mapas[fila][columna][0] != '0');

    do {
        x = rand() % 4;
    } while (Tesoro[x] != false);

    if (x == 0){
        strcpy(Mapas[fila][columna], "X");
    }else if (x == 1){
        strcpy(Mapas[fila][columna], "Y");
    }else if (x == 2){
        strcpy(Mapas[fila][columna], "W");
    }else{
        strcpy(Mapas[fila][columna], "Z");
    }
    Tesoro[x] = true;
}

void InsertarEspecial(char Mapas[5][5][3]){
    /*
    Se inserta un caracter especial en el Mapa. Teniendo un respectivo juego de probabilidades para saber cual se agrega.
    */

    int fila, columna;
    do {
        fila = rand() % 5;
        columna = rand() % 5;
    } while (Mapas[fila][columna][0] != '0');
    
    int caracterAleatorio = rand() % 4;
    
    if (caracterAleatorio == 0){
        strcpy(Mapas[fila][columna], "+");
    } else if (caracterAleatorio == 1){
        strcpy(Mapas[fila][columna], "-");
    } else if (caracterAleatorio == 2){
        strcpy(Mapas[fila][columna], "T");
    } else {
        strcpy(Mapas[fila][columna], "C");
    }
}

void TesoroOrEspecial(char Mapas[5][5][3]){
    /*
    Por cada nevo tablero a ingresar, pasa por esta funcion para ver si se agrega un Tesoro o un Caracter Especial.
    Existe una logica para que si o si se ingresen los 4 Tesoros a los tableros. 
    */

    int flags = 0;
    int tesoro = 0;

    int probabilidad = rand() % 2;

    for (int x = 0 ; x < 8 ; x++){
        if(Flags[x] == false){
            flags++;
        }
    }

    for (int x = 0 ; x < 4 ; x++){
        if(Tesoro[x] == false){
            tesoro++;
        }
    }

    if (flags == tesoro){
        InsertarTesoro(Mapas);
    }else{
        if(probabilidad == 0){
            InsertarTesoro(Mapas);
        }else{
            InsertarEspecial(Mapas); 
        }
    }
}

int LugarParaInsertar(int fila, int columna, char matrizGrande[85][85][3]) {
    /*
    Verifica en que lugar se puede insertar el tablero solicitado.
    */

    int FilaMenos = fila - 1;
    int FilaMas = fila + 1;
    int ColumnaMenos = columna - 1;
    int ColumnaMas = columna + 1;

    if (FilaMenos >= 0 && strcmp(matrizGrande[FilaMenos][columna], " ") == 0) {
        return -1;
    } else if (FilaMas >= 0 && strcmp(matrizGrande[FilaMas][columna], " ") == 0) {
        return 1;
    } else if (ColumnaMenos >= 0 && strcmp(matrizGrande[fila][ColumnaMenos], " ") == 0) {
        return -2;
    } else if (ColumnaMas >= 0 && strcmp(matrizGrande[fila][ColumnaMas], " ") == 0) {
        return 2;
    } else{
        printf("\n");
        return 0;
    }
}

void BuscarCompatibles(int fila, int columna, char matrizGrande[85][85][3]) {
    /*
    Busca la posicion a insertar el tablero en el mapa. Luego ve que tablero cumple con las condiciones para ser insertado
    y luego lo inserta.
    */

    char Mapas[8][5][5][3];

    for (int i = 0; i < 8; i++) {
        char nombreArchivo[20];
        sprintf(nombreArchivo, "tablero%d.txt", i + 1);
        LectorTXT(nombreArchivo, Mapas[i]);
    }

    int insertarFila, insertarColumna;

    int Resultado;

    Resultado = LugarParaInsertar(fila, columna, matrizGrande);

    if (Resultado == -1){
        insertarFila = fila - 1;
        insertarFila = insertarFila / 5;
        insertarFila = insertarFila * 5;
        insertarColumna = columna - 2;
        fila = 4;
        columna = 2;

    }else if (Resultado == 1){
        insertarFila = fila + 1;
        insertarFila = insertarFila / 5;
        insertarFila = insertarFila * 5;
        insertarColumna = columna - 2;
        fila = 0;
        columna = 2;

    }else if (Resultado == -2){
        insertarColumna = columna - 1;
        insertarColumna = insertarColumna / 5;
        insertarColumna = insertarColumna * 5;
        insertarFila = fila - 2;
        fila = 2;
        columna = 4;

    }else if (Resultado == 2){
        insertarColumna = columna + 1;
        insertarColumna = insertarColumna / 5;
        insertarColumna = insertarColumna * 5;
        insertarFila = fila - 2;
        fila = 2;
        columna = 0;

    }

    for (int i = 0; i < 8; i++) {
        if (!Flags[i] && Resultado != 0 && strcmp(Mapas[i][fila][columna], "B") == 0) {
            TesoroOrEspecial(Mapas[i]);
            InsertarMapas(insertarFila, insertarColumna, Mapas[i], matrizGrande);
            Flags[i] = true;
            break;
        }
        else if (!Flags[i] && Resultado != 0 && strcmp(Mapas[i][fila][columna], "1") == 0) {
            TesoroOrEspecial(Mapas[i]);
            InsertarMapas(insertarFila, insertarColumna, Mapas[i], matrizGrande);
            Flags[i] = true;
            break;
        }
        else if (!Flags[i] && Resultado != 0 && strcmp(Mapas[i][fila][columna], "2") == 0) {
            TesoroOrEspecial(Mapas[i]);
            InsertarMapas(insertarFila, insertarColumna, Mapas[i], matrizGrande);
            Flags[i] = true;
            break;
        }else if (!Flags[i] && Resultado != 0 && strcmp(Mapas[i][fila][columna], "3") == 0) {
            TesoroOrEspecial(Mapas[i]);
            InsertarMapas(insertarFila, insertarColumna, Mapas[i], matrizGrande);
            Flags[i] = true;
            break;
        }else if (!Flags[i] && Resultado != 0 && strcmp(Mapas[i][fila][columna], "4") == 0) {
            TesoroOrEspecial(Mapas[i]);
            InsertarMapas(insertarFila, insertarColumna, Mapas[i], matrizGrande);
            Flags[i] = true;
            break;
        }
    }
}

void asignarOpcionesAleatoriamente() {
    /*
    Asigna las cartas de manera aleratoria a los jugadores.
    */
    int buscarCount = 0, escaleraCount = 0;

    for (int i = 0; i < 4; i++) {
        opciones_adicionales[i] = 0;
    }

    while (buscarCount < 2 || escaleraCount < 2) {
        for (int i = 0; i < 4; i++) {
            if (opciones_adicionales[i] == 0) {
                int opcion = (rand() % 2) + 5;  

                if (opcion == 5 && buscarCount < 2) {
                    opciones_adicionales[i] = 5;
                    buscarCount++;
                } else if (opcion == 6 && escaleraCount < 2) {
                    opciones_adicionales[i] = 6;
                    escaleraCount++;
                }
            }
        }
    }
}

void Teletransporte(int *Pos_Incial_x, int *Pos_Incial_y, int Pos_Final_x, int Pos_Final_y, char Mapa[FILAS_GRANDE][COLUMNAS_GRANDE][3]) {
    /*
    Toma la pos incial de uno de los jugadores y lo teletransporta a la nueva posicion.
    */

    if (Mapa[Pos_Final_x][Pos_Final_y][0] == '1' ||
        Mapa[Pos_Final_x][Pos_Final_y][0] == '2' ||
        Mapa[Pos_Final_x][Pos_Final_y][0] == '3' ||
        Mapa[Pos_Final_x][Pos_Final_y][0] == '4') {
        
        printf("Teletransporte fallido :p, ya hay un jugador en esa casilla.\n");
        return;  
    }

    // Guardamos el objeto en la posición inicial en una variable temporal
    char tempObj[3];
    strcpy(tempObj, Mapa[*Pos_Incial_x][*Pos_Incial_y]);

    // Cambiamos el objeto en la posición inicial con el objeto en la posición final
    strcpy(Mapa[*Pos_Incial_x][*Pos_Incial_y], Mapa[Pos_Final_x][Pos_Final_y]);

    // Cambiamos el objeto en la posición final con el objeto guardado en la variable temporal
    strcpy(Mapa[Pos_Final_x][Pos_Final_y], tempObj);

    // Actualizamos las posiciones iniciales
    *Pos_Incial_x = Pos_Final_x;
    *Pos_Incial_y = Pos_Final_y;
}

void moverJugador(int jugador, char Mapa[FILAS_GRANDE][COLUMNAS_GRANDE][3], int direccion, int distancia, char *sobre, int *posx, int *posy, int opciones_adicionales[])
 {
    /*
    Mover al jugador y al mismo tiempo almacena los caracteres de llegada debajo de cada jugador. 
    */

    int x = -1, y = -1;
    char jugadorChar[3];  

    sprintf(jugadorChar, "%d", jugador);  

    for (int i = 0; i < FILAS_GRANDE; i++) {
        for (int j = 0; j < COLUMNAS_GRANDE; j++) {
            if (strcmp(Mapa[i][j], jugadorChar) == 0) { 
                x = i;
                y = j;
                break;
            }
        }
        if (x != -1) break;  
    }

    if (x == -1 || y == -1) {
        printf("Jugador no encontrado en el Mapa.\n");
        return;
    }

    char prevChar[3];
    prevChar[0] = *sobre;
    prevChar[1] = '\0';

    char temp[3];
    switch(direccion) {
        case 1:  // Arriba
            for (int i = 0; i < distancia; i++) {
                if (x < FILAS_GRANDE - 1 && 
                    (strcmp(Mapa[x-1][y], "0") == 0 || strcmp(Mapa[x-1][y], "B") == 0 ||
                     strcmp(Mapa[x-1][y], "+") == 0 || strcmp(Mapa[x-1][y], "-") == 0 ||
                     strcmp(Mapa[x-1][y], "X") == 0 || strcmp(Mapa[x-1][y], "Y") == 0 ||
                     strcmp(Mapa[x-1][y], "W") == 0 || strcmp(Mapa[x-1][y], "Z") == 0 || 
                    (strcmp(Mapa[x-1][y], "E") == 0 && opciones_adicionales[jugador - 1] == 6) || strcmp(Mapa[x-1][y], "S") == 0 ||
                     strcmp(Mapa[x-1][y], "C") == 0 || strcmp(Mapa[x-1][y], "T") == 0)) {
                    strcpy(temp, Mapa[x-1][y]);  
                    strcpy(Mapa[x-1][y], jugadorChar);  
                    Mapa[x][y][0] = *sobre;
                    Mapa[x][y][1] = '\0';
                    *sobre = temp[0];  
                    x--;
                } else {
                    break;
                }
            }
            break;

        case 2:  // Abajo
            for (int i = 0; i < distancia; i++) {
                if (x < FILAS_GRANDE - 1 && 
                    (strcmp(Mapa[x+1][y], "0") == 0 || strcmp(Mapa[x+1][y], "B") == 0 ||
                     strcmp(Mapa[x+1][y], "+") == 0 || strcmp(Mapa[x+1][y], "-") == 0 ||
                     strcmp(Mapa[x+1][y], "X") == 0 || strcmp(Mapa[x+1][y], "Y") == 0 ||
                     strcmp(Mapa[x+1][y], "W") == 0 || strcmp(Mapa[x+1][y], "Z") == 0 || 
                    (strcmp(Mapa[x+1][y], "E") == 0 && opciones_adicionales[jugador - 1] == 6) || strcmp(Mapa[x+1][y], "S") == 0 ||
                     strcmp(Mapa[x+1][y], "C") == 0 || strcmp(Mapa[x+1][y], "T") == 0)) {

                    strcpy(temp, Mapa[x+1][y]);  
                    strcpy(Mapa[x+1][y], jugadorChar);  
                    Mapa[x][y][0] = *sobre;
                    Mapa[x][y][1] = '\0';
                    *sobre = temp[0];  
                    x++;
                } else {
                    break;
                }
            }
            break;

        case 3:  // Izquierda
            for (int i = 0; i < distancia; i++) {
                if (y > 0 && 
                    (strcmp(Mapa[x][y-1], "0") == 0 || strcmp(Mapa[x][y-1], "B") == 0 ||
                     strcmp(Mapa[x][y-1], "+") == 0 || strcmp(Mapa[x][y-1], "-") == 0 ||
                     strcmp(Mapa[x][y-1], "X") == 0 || strcmp(Mapa[x][y-1], "Y") == 0 ||
                     strcmp(Mapa[x][y-1], "W") == 0 || strcmp(Mapa[x][y-1], "Z") == 0 || 
                    (strcmp(Mapa[x][y-1], "E") == 0 && opciones_adicionales[jugador - 1] == 6) || strcmp(Mapa[x][y-1], "S") == 0 ||
                     strcmp(Mapa[x][y-1], "C") == 0 || strcmp(Mapa[x][y-1], "T") == 0)) {

                    strcpy(temp, Mapa[x][y-1]);  
                    strcpy(Mapa[x][y-1], jugadorChar);  
                    Mapa[x][y][0] = *sobre;
                    Mapa[x][y][1] = '\0';
                    *sobre = temp[0];  
                    y--;
                } else {
                    break;
                }
            }
            break;

        case 4:  // Derecha
            for (int i = 0; i < distancia; i++) {
                if (y < COLUMNAS_GRANDE - 1 && 
                    (strcmp(Mapa[x][y+1], "0") == 0 || strcmp(Mapa[x][y+1], "B") == 0 ||
                     strcmp(Mapa[x][y+1], "+") == 0 || strcmp(Mapa[x][y+1], "-") == 0 ||
                     strcmp(Mapa[x][y+1], "X") == 0 || strcmp(Mapa[x][y+1], "Y") == 0 ||
                     strcmp(Mapa[x][y+1], "W") == 0 || strcmp(Mapa[x][y+1], "Z") == 0 || 
                    (strcmp(Mapa[x][y+1], "E") == 0 && opciones_adicionales[jugador - 1] == 6) || strcmp(Mapa[x][y+1], "S") == 0||
                     strcmp(Mapa[x][y+1], "C") == 0 || strcmp(Mapa[x][y+1], "T") == 0)) {

                    strcpy(temp, Mapa[x][y+1]);  
                    strcpy(Mapa[x][y+1], jugadorChar);  
                    Mapa[x][y][0] = *sobre;
                    Mapa[x][y][1] = '\0';
                    *sobre = temp[0];  
                    y++;
                } else {
                    break;
                }
            }
            break;

        default:
            printf("Direccion no valida.\n");
            return;
    }

    *posx = x;
    *posy = y;
}

void imprimirCadenaPausada(const char *cadena) {
    /*
    Imprime algunas cadenas de caracteres lentamente.
    */
    int microsegundos = 25;
    for (int i = 0; cadena[i] != '\0'; i++) {
        putchar(cadena[i]);
        fflush(stdout);
        usleep(microsegundos);
    }
}

void mostrarInterfaz(int jugador) {
    /*
    Imprime un menu de interaccion para cada jugador.
    */
    char mensaje[200];

    snprintf(mensaje, sizeof(mensaje), "\nJUGADOR %d, elige una opción:\n", jugador);
    imprimirCadenaPausada(mensaje);

    imprimirCadenaPausada("0. Saltar Turno.\n");
    imprimirCadenaPausada("1. Ir arriba.\n");
    imprimirCadenaPausada("2. Ir abajo.\n");
    imprimirCadenaPausada("3. Ir a la izquierda.\n");
    imprimirCadenaPausada("4. Ir a la derecha.\n");

    if (opciones_adicionales[jugador - 1] == 5) {
        imprimirCadenaPausada("5. Buscar.\n");
    } else if (opciones_adicionales[jugador - 1] == 6) {
        imprimirCadenaPausada("5. Usar escalera.\n");
    }
}

/* M A I N */
int main() {
    mostrarSimbolos();
    printf("\n");
    CrearMatriz(Mapa);

    LectorTXT("Inicio.txt", Inicio);
    InsertarMatriz(Mapa, Inicio);
    CambiarCaracteres(Inicio);
    InsertarMatriz(Mapa, Inicio);

    srand(time(NULL));  // Inicializa la semilla para números aleatorios
    asignarOpcionesAleatoriamente();
    

    char escalera1[] = "\n\nEl Jugador 1 tiene la carta ESCALERA\n";
    char buscar1[] = "El Jugador 1 tiene la carta BUSCAR\n";

    char escalera2[] = "El Jugador 2 tiene la carta ESCALERA\n";
    char buscar2[] = "El Jugador 2 tiene la carta BUSCAR\n";

    char escalera3[] = "El Jugador 3 tiene la carta ESCALERA\n";
    char buscar3[] = "El Jugador 3 tiene la carta BUSCAR\n";

    char escalera4[] = "El Jugador 4 tiene la carta ESCALERA\n";
    char buscar4[] = "El Jugador 4 tiene la carta BUSCAR\n\n";

    if(opciones_adicionales[0] == 5){
        imprimirCadenaPausada(buscar1);
    }else{
        imprimirCadenaPausada(escalera1);
    }
    if(opciones_adicionales[1] == 5){
        imprimirCadenaPausada(buscar2);
    }else{
        imprimirCadenaPausada(escalera2);
    }
    if(opciones_adicionales[2] == 5){
        imprimirCadenaPausada(buscar3);
    }else{
        imprimirCadenaPausada(escalera3);
    }
    if(opciones_adicionales[3] == 5){
        imprimirCadenaPausada(buscar4);
    }else{
        imprimirCadenaPausada(escalera4);
    }


    int totalturnos = 15;
    int pipes[4][2];
 

    char buffer[100];
    

    int tepes[4][2];
    for (int i = 0; i < 4; i++) {
        tepes[i][0] = -1;
        tepes[i][1] = -1;
    }

    
    char Jugador1Sobre = 'S';
    char Jugador2Sobre = 'S';
    char Jugador3Sobre = 'S';
    char Jugador4Sobre = 'S';
    int J1x, J1y, J2x, J2y, J3x, J3y, J4x, J4y;
    bool J1tesoro = false;
    bool J2tesoro = false;
    bool J3tesoro = false;
    bool J4tesoro = false;

    int tp = 0;
    for (int i = 0; i < 4; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("Error al crear la tubería");
            return 1;
        }
    }

   
    for (int i = 0; i < 3; i++) {
        
        if (fork() == 0) { // Hijos
            for (int turn = 0; turn < totalturnos; turn++) {

                
                close(pipes[i][1]);          
                close(pipes[3][0]);          


                read(pipes[i][0], &buffer, 1);  

                usleep(200000);  
                printf("\n\n");
                mostrarInterfaz(i+2);

                // Captura la elección del hijo
                printf("Escribe tu elección, jugador %d: ", i + 2);
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';  // Eliminar el salto de línea de la elección

                char distancia[10];
                if(buffer[0] == '1'||buffer[0] == '2'||buffer[0] == '3'||buffer[0] == '4'){
                    printf("Distancia: ");
                    fgets(distancia, sizeof(distancia), stdin);
                    distancia[strcspn(distancia, "\n")] = '\0';  // Eliminar el salto de línea de la distancia
                }
                
                // Concatenar elección y distancia en buffer
                strcat(buffer, " ");
                strcat(buffer, distancia);

                write(pipes[3][1], buffer, strlen(buffer) + 1);
                
                if (i < 2) {  // Si no es el último hijo
                    write(pipes[i + 1][1], "1", 1);  // Pasa el turno al siguiente hijo
                    
                }
            }
            return 0;
        }
    }
    if (fork() != 0) {
        close(pipes[3][1]);    // Extremo de escritura de la pipe general

        for (int i = 0; i < 3; i++) {
            close(pipes[i][0]);  // Extremos de lectura de las pipes de los hijos
        }
        for (int turn = 0; turn < totalturnos; turn++) {

            // Mostrar la matriz al inicio del turno
            printf("\n\n");
            llenarTPs(Mapa, tepes);

            char mensaje[200];
            snprintf(mensaje, sizeof(mensaje),"TURNOS TOTALES: %d\n", totalturnos);
            imprimirCadenaPausada(mensaje);

            char mensaje2[200];
            snprintf(mensaje2, sizeof(mensaje2),"TURNO: %d \n\n", turn + 1);
            imprimirCadenaPausada(mensaje2);

            ImprimirMatriz(Mapa);
            mostrarInterfaz(1);
            
            // Acción del jugador 1 y actualización del mapa
            int direccion, distancia;
            printf("Escribe tu elección, jugador 1: ");
            scanf("%d", &direccion);
            if (direccion >= 1 && direccion <= 4) { 
                printf("Distancia: ");
                scanf("%d", &distancia); 
                moverJugador(1, Mapa, direccion, distancia, &Jugador1Sobre, &J1x, &J1y, opciones_adicionales);
                
                
            } else if (direccion == 5) {
                if(opciones_adicionales[0] == 6){
                    if(Jugador1Sobre == 'E'){
                        Jugador1Sobre = '0';
                    }else{
                        printf("Debes estar parado sobre una casilla escalera(Puerta) para usar esta carta.\n");
                    }
                }else if(opciones_adicionales[0] == 5){
                    if(Jugador1Sobre == 'B'){
                        BuscarCompatibles(J1x,J1y, Mapa);
                    }else{
                        printf("Debes estar parado sobre una casilla de Buscar para usar esta carta.\n");
                    }
                }
                    
                
            }
            getchar();  

            int totalCamaras = buscarCamaras(Mapa);
            printf("Total de cámaras encontradas: %d\n", totalCamaras);

            // Mostrar la matriz después de la acción del jugador 1
            ImprimirMatriz(Mapa);  

            write(pipes[0][1], "1", 1);
            
            for (int i = 0; i < 3; i++) {
                char movimiento[10];
                read(pipes[3][0], movimiento, sizeof(movimiento));

                int direccionHijo = movimiento[0] - '0';

                if (direccionHijo >= 1 && direccionHijo <= 4) {
                    char* end;  // Para almacenar el final de la parte numérica de la cadena
                    int distanciaHijo = strtol(&movimiento[2], &end, 10);

                    switch(i) {
                        case 0: // Jugador 2
                            moverJugador(i + 2, Mapa, direccionHijo, distanciaHijo, &Jugador2Sobre, &J2x, &J2y, opciones_adicionales);
                            

                            break;
                        case 1: // Jugador 3
                            moverJugador(i + 2, Mapa, direccionHijo, distanciaHijo, &Jugador3Sobre, &J3x, &J3y, opciones_adicionales);
                            

                            break;
                        case 2: // Jugador 4
                            moverJugador(i + 2, Mapa, direccionHijo, distanciaHijo, &Jugador4Sobre, &J4x, &J4y, opciones_adicionales);
                            
                            break;
                    }
                } else if (direccionHijo == 5) {
                    switch(i) {
                        case 0: // Jugador 2
                            if(opciones_adicionales[1] == 6){
                                if(Jugador2Sobre == 'E'){
                                    Jugador2Sobre = '0';
                                }else{
                                    printf("Debes estar parado sobre una casilla escalera(Puerta) para usar esta carta.\n");
                                }
                            }else if(opciones_adicionales[1] == 5){
                                if(Jugador2Sobre == 'B'){
                                    BuscarCompatibles(J2x,J2y, Mapa);
                                }else{
                                    printf("Debes estar parado sobre una casilla de Buscar para usar esta carta.\n");
                                }
                            }
                            break;
                        case 1: // Jugador 3
                            if(opciones_adicionales[2] == 6){
                                if(Jugador3Sobre == 'E'){
                                    Jugador3Sobre = '0';
                                }else{
                                    printf("Debes estar parado sobre una casilla escalera(Puerta) para usar esta carta.\n");
                                }
                            }else if(opciones_adicionales[2] == 5){
                                if(Jugador3Sobre == 'B'){
                                    BuscarCompatibles(J3x,J3y, Mapa);
                                }else{
                                    printf("Debes estar parado sobre una casilla de Buscar para usar esta carta.\n");
                                }
                            }
                            break;
                        case 2: // Jugador 4
                            if(opciones_adicionales[3] == 6){
                                if(Jugador4Sobre == 'E'){
                                    Jugador4Sobre = '0';
                                }else{
                                    printf("Debes estar parado sobre una casilla escalera(Puerta) para usar esta carta.\n");
                                }
                            }else if(opciones_adicionales[3] == 5){
                                if(Jugador4Sobre == 'B'){
                                    BuscarCompatibles(J4x,J4y, Mapa);
                                }else{
                                    printf("Debes estar parado sobre una casilla de Buscar para usar esta carta.\n");
                                }
                            }
                            break;
                    }
                }else if(direccionHijo == 0){

                    printf("\n");
                }else{
                    printf("Opcion invalida, turno cancelado\n");
                }




                //Borrar camaras
                if(Jugador1Sobre == 'C'){
                    Jugador1Sobre = '0';
                }
                if(Jugador2Sobre == 'C'){
                    Jugador2Sobre = '0';
                }
                if(Jugador3Sobre == 'C'){
                    Jugador3Sobre = '0';
                }
                if(Jugador4Sobre == 'C'){
                    Jugador4Sobre = '0';
                }

                //Contar camaras
                int totalCamaras = buscarCamaras(Mapa);
                printf("Total de cámaras encontradas: %d\n", totalCamaras);

                //Sumar o restar turnos dependiendo si el jugador se para sobre un + o -
                //Si la cantidad de Camaras es mayor o igual a 2 entonces no se suman turnos
                if(Jugador1Sobre == '+'){

                    if(totalCamaras >= 2){
                        printf("Demasiadas camaras, deben ser maximo 2");
                    }else{
                        totalturnos += 5;
                        Jugador1Sobre = '0';
                    }
                    
                    
                }else if(Jugador1Sobre == '-'){
                    totalturnos -= 3;
                    Jugador1Sobre = '0';
                }
                
                if(Jugador2Sobre == '+'){
                    if(totalCamaras >= 2){
                        printf("Demasiadas camaras, deben ser maximo 2");
                    }else{
                        totalturnos += 5;
                        Jugador2Sobre = '0';
                    }   
                }else if(Jugador2Sobre == '-'){
                    totalturnos -= 3;
                    Jugador2Sobre = '0';
                }
                
                if(Jugador3Sobre == '+'){
                    if(totalCamaras >= 2){
                        printf("Demasiadas camaras, deben ser maximo 2");
                    }else{
                        totalturnos += 5;
                        Jugador3Sobre = '0';
                    }
                }else if(Jugador3Sobre == '-'){
                    totalturnos -= 3;
                    Jugador3Sobre = '0';
                }
                
                if(Jugador4Sobre == '+'){
                    if(totalCamaras >= 2){
                        printf("Demasiadas camaras, deben ser maximo 2");
                    }else{
                        totalturnos += 5;
                        Jugador4Sobre = '0';
                    }
                }else if(Jugador4Sobre == '-'){
                    totalturnos -= 3;
                    Jugador4Sobre = '0';
                }
                

                //Comprobar tesoros

                if(Jugador1Sobre == 'X'){
                    J1tesoro = true;
                    Jugador1Sobre = '0';
                }
                if(Jugador2Sobre == 'Y'){
                    J2tesoro = true;
                    Jugador2Sobre = '0';
                }
                if(Jugador3Sobre == 'W'){
                    J3tesoro = true;
                    Jugador3Sobre = '0';
                }
                if(Jugador4Sobre == 'Z'){
                    J4tesoro = true;
                    Jugador4Sobre = '0';
                }



                // Mostrar la matriz después de la acción de cada hijo
                ImprimirMatriz(Mapa);
            }

                if(Jugador1Sobre == 'T'){
                    printf("El Jugador 1 cayo en una casilla de Teletransporte este turno!!\n");
                    printf("Elige donde teletransportarte Jugador 1: \n");
                    mostrarTPS(tepes);
                    scanf("%d", &tp);
                    Teletransporte(&J1x, &J1y, tepes[tp-1][0],tepes[tp-1][1], Mapa );
                }

                if(Jugador2Sobre == 'T'){
                    printf("El Jugador 2 cayo en una casilla de Teletransporte este turno!!\n");
                    printf("Elige donde teletransportarte Jugador 2: \n");
                    mostrarTPS(tepes);
                    scanf("%d", &tp);
                    Teletransporte(&J2x, &J2y, tepes[tp-1][0],tepes[tp-1][1], Mapa );
                }


                if(Jugador3Sobre == 'T'){
                    printf("El Jugador 3 cayo en una casilla de Teletransporte este turno!!\n");
                    printf("Elige donde teletransportarte Jugador 3: \n");
                    mostrarTPS(tepes);
                    scanf("%d", &tp);
                    Teletransporte(&J3x, &J3y, tepes[tp-1][0],tepes[tp-1][1], Mapa );
                }
                if(Jugador4Sobre == 'T'){
                    printf("El Jugador 4 cayo en una casilla de Teletransporte este turno!!\n");
                    printf("Elige donde teletransportarte Jugador 4: \n");
                    mostrarTPS(tepes);
                    scanf("%d", &tp);
                    Teletransporte(&J4x, &J4y, tepes[tp-1][0],tepes[tp-1][1], Mapa );
                }

            

                if(
                    J1tesoro == true && Jugador1Sobre == 'S' &&
                    J2tesoro == true && Jugador2Sobre == 'S' &&
                    J3tesoro == true && Jugador3Sobre == 'S' &&
                    J4tesoro == true && Jugador4Sobre == 'S'

                ){
                    printf("\n\nFELICIDADES!! TODOS LOS JUGADORES LOGRARON ESCAPAR CON EL TESORO\n\n");
                    printf("Congratulations ^.^/\n\n");
                    return 0;
                }
                
            }
            printf("\n\nLo siento :c se les acabaron los turnos..\n\n");
            printf("Game Over\n\n");
            return 0;
    }
    
    for (int i = 0; i < 3; i++) {
        
        wait(NULL);
    }

    for (int i = 0; i < 4; i++) {
        close(pipes[i][1]);  
    }
    close(pipes[3][0]);  
    

}
