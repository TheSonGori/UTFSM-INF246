LABORATORIO 2
INTEGRANTES:
 - Nombre: Elias Valle       | Rol: 202173537-2 | Paralelo: 200
 - Nombre: Javiera Gutierrez | Rol: 202173626-3 | Paralelo: 200

INSTRUCCION DE EJECUCION:
 - 'make main' para que se ejecute el codigo.
 - 'make clean' para que se borre el ejecutable del codigo.

SUPOSICIONES Y ACLARACIONES:
 - Se asume que los archivos .txt tienen el siguiente formato : 
   'tableroX.txt' - donde X es un valor entre 1 y 8.
 - Que las 'E' escaleras son puertas, por lo tanto, los jugadores que tengan esta carta pueden pasar por ella, los que no tienen la carta
   interactuaran con las 'E' como si fueran obstaculos. Los jugadores que tengan la carta y la activen estando sobre una 'E' abriran el paso para los demas.
 - Se asume que se van a generar 4 Tps como maximo (por el juego de probabilidades donde si o si tienen que estar los 4 tesoros).
 - Para accionar las cartas ya sea la de 'Buscar' o 'Escalera', los jugadores tienen que estar encima de ella.
 - El orden de los Tp es acorde a su orden de ingreso a los tableros.
 - Los Tp se hacen siempre al final de cada turno.
 - Para desactivar camaras, agarrar tesoros y usar la carta escalera, se borra el caracter y se cambia por un '0'.
 - Solo el jugador correspondiente puede agarrar el tesoro que le pertece, en el caso de la tarea seria (1,X);(2,Y);(3,W) y (4,Z).
 - Para ganar cada jugador debe haber agarrado su tesoro y estar de pie sobre alguna de las casillas 'S' que se generaron en el inicio.

