public class Nada {
    private char[][] matriz;
    private String palabra;
    private int dimension;
    private long tiempo;

    public Nada(char[][] matriz, String palabra, int dimension, long tiempo) {
        this.matriz = matriz;
        this.palabra = palabra;
        this.dimension = dimension;
        this.tiempo = tiempo;
    }

    public void main() {
        buscarHorizontal();
        buscarVertical();
    }

    public void buscarHorizontal() {
        int len = palabra.length();

        for (int i = 0; i < dimension; i++) {
            for (int j = 0; j <= dimension - len; j++) {
                StringBuilder sb = new StringBuilder();
                for (int k = 0; k < len; k++) {
                    sb.append(matriz[i][j + k]);
                }
                if (sb.toString().equals(palabra)) {
                    long Time = System.currentTimeMillis();
                    Time = Time - tiempo;
                    System.out.println("\nNADA");
                    System.out.println("Palabra encontrada en fila " + (i + 1) + ", columna " + (j + 1));
                    System.out.println("Tiempo en encontrar la palabra: " + Time + " [ms]\n");
                    System.out.println("\n\nLa palabra estaba de forma HORIZONTAL");
                }
            }
        }
    }

    public void buscarVertical() {
        int len = palabra.length();

        for (int i = 0; i < dimension; i++) {
            for (int j = 0; j <= dimension - len; j++) {
                StringBuilder sb = new StringBuilder();
                for (int k = 0; k < len; k++) {
                    sb.append(matriz[j + k][i]);
                }
                if (sb.toString().equals(palabra)) {
                    long Time = System.currentTimeMillis();
                    Time = Time - tiempo;
                    System.out.println("\nNADA");
                    System.out.println("Palabra encontrada en fila " + (j + 1) + ", columna " + (i + 1));
                    System.out.println("Tiempo en encontrar la palabra: " + Time + " [ms]\n");
                    System.out.println("\n\nLa palabra estaba de forma VERTICAL");
                }
            }
        }
    }
}
