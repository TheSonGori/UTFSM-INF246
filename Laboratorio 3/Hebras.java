public class Hebras extends Thread {
    private char[][] matriz;
    private String palabra;
    private int startRow, endRow, startCol, endCol, dimension;
    private long tiempo;

    public Hebras(char[][] matriz, String palabra, int startRow, int endRow, int startCol, int endCol, int dimension, long tiempo) {
        this.matriz = matriz;
        this.palabra = palabra;
        this.startRow = startRow;
        this.endRow = endRow;
        this.startCol = startCol;
        this.endCol = endCol;
        this.dimension = dimension;
        this.tiempo = tiempo;
    }

    private void buscarPalabraEnSubmatriz() {
        int len = palabra.length();

        // Buscar horizontalmente de izquierda a derecha
        boolean palabraEncontrada = true;
        for(int x = startRow; x<endRow ; x++){
            for (int i = 0; i < len; i++) {
                if (matriz[x][startCol + i] != palabra.charAt(i)) {
                    palabraEncontrada = false;
                    break;
                }
            }
            if (palabraEncontrada) {
                long Time = System.currentTimeMillis();
                Time = Time - tiempo;
                System.out.println("\nHEBRAS");
                System.out.println("Palabra encontrada en fila " + (x+1) + ", columna " + (startCol+1));
                System.out.println("Tiempo en encontrar la palabra: " + Time + " [ms]\n");
            }
            palabraEncontrada = true;
        }

        // Buscar verticalmente de arriba hacia abajo
        palabraEncontrada = true;
        for (int x = startCol; x<endCol ; x++){
            for (int i = 0; i < len; i++) {
                if (matriz[startRow + i][x] != palabra.charAt(i)) {
                    palabraEncontrada = false;
                    break;
                }
            }
            if (palabraEncontrada) {
                long Time = System.currentTimeMillis();
                Time = Time - tiempo;
                System.out.println("\nHEBRAS");
                System.out.println("Palabra encontrada en fila " + (startRow+1) + ", columna " + (x+1));
                System.out.println("Tiempo en encontrar la palabra: " + Time + " [ms]\n");
            }
            palabraEncontrada = true;
        }        
    }

    @Override
    public void run() {
        int tam = palabra.length();

        if (dimension == tam){
            buscarPalabraEnSubmatriz();
        }else{
            //(matriz, String palabra, int startRow, int endRow, int startCol, int endCol, int dimension)
            Hebras A = new Hebras(matriz, palabra, startRow, startRow + dimension/2, startCol,startCol + dimension/2, dimension/2, tiempo);

            Hebras B = new Hebras(matriz, palabra, startRow, startRow + dimension/2, startCol + dimension/2, endCol, dimension/2, tiempo);

            Hebras C = new Hebras(matriz, palabra, startRow + dimension/2, endRow , startCol, startCol + dimension/2, dimension/2, tiempo);

            Hebras D = new Hebras(matriz, palabra, startRow + dimension/2, endRow , startCol + dimension/2, endCol, dimension/2, tiempo);

            A.start();
            B.start();
            C.start();
            D.start();


            try{
                A.join();
                B.join();
                C.join();
                D.join();

            }catch(InterruptedException e){

            }
        }
    }

}
