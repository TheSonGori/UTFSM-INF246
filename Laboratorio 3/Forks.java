import java.util.concurrent.RecursiveAction;

public class Forks extends RecursiveAction {
    private char[][] matrix;
    private String palabra;
    private int startRow, endRow, startCol, endCol;
    private int dimension;
    private long startTime;

    public Forks(char[][] matrix, String palabra, int startRow, int endRow, int startCol, int endCol, int dimension,
            long startTime) {
        this.matrix = matrix;
        this.palabra = palabra;
        this.startRow = startRow;
        this.endRow = endRow;
        this.startCol = startCol;
        this.endCol = endCol;
        this.dimension = dimension;
        this.startTime = startTime;
    }

    @Override
    protected void compute() {
        int len = palabra.length();

        if (dimension == len) {
            buscarPalabraEnSubmatriz();
        } else {
            int midRow = (startRow + endRow) / 2;
            int midCol = (startCol + endCol) / 2;

            Forks A = new Forks(matrix, palabra, startRow, midRow, startCol, midCol, dimension / 2, startTime);
            Forks B = new Forks(matrix, palabra, startRow, midRow, midCol, endCol, dimension / 2, startTime);
            Forks C = new Forks(matrix, palabra, midRow, endRow, startCol, midCol, dimension / 2, startTime);
            Forks D = new Forks(matrix, palabra, midRow, endRow, midCol, endCol, dimension / 2, startTime);

            A.fork();
            B.fork();
            C.fork();
            D.fork();

            A.join();
            B.join();
            C.join();
            D.join();

        }
    }

    private void buscarPalabraEnSubmatriz() {
        int len = palabra.length();

        // Buscar horizontalmente de izquierda a derecha
        boolean palabraEncontrada = true;
        for (int x = startRow; x < endRow; x++) {
            for (int i = 0; i < len; i++) {
                if (matrix[x][startCol + i] != palabra.charAt(i)) {
                    palabraEncontrada = false;
                    break;
                }
            }
            if (palabraEncontrada) {
                long tiempoActual = System.currentTimeMillis();
                long tiempoTranscurrido = tiempoActual - startTime;
                System.out.println("\nFORKS");
                System.out.println("Palabra encontrada en fila " + (x + 1) + ", columna " + (startCol + 1));
                System.out.println("Tiempo en encontrar la palabra: " + tiempoTranscurrido + " [ms]\n");
            }
            palabraEncontrada = true;
        }

        // Buscar verticalmente de arriba hacia abajo
        palabraEncontrada = true;
        for (int x = startCol; x < endCol; x++) {
            for (int i = 0; i < len; i++) {
                if (matrix[startRow + i][x] != palabra.charAt(i)) {
                    palabraEncontrada = false;
                    break;
                }
            }
            if (palabraEncontrada) {
                long tiempoActual = System.currentTimeMillis();
                long tiempoTranscurrido = tiempoActual - startTime;
                System.out.println("\nFORKS");
                System.out.println("Palabra encontrada en fila " + (startRow + 1) + ", columna " + (x + 1));
                System.out.println("Tiempo en encontrar la palabra: " + tiempoTranscurrido + " [ms]\n");
            }
            palabraEncontrada = true;
        }
    }
}
