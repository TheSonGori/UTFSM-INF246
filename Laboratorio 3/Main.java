import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.concurrent.ForkJoinPool;

public class Main {
    public static void main(String[] args) {
        try {
            File file = new File("sopa_de_letras.txt");

            if (!file.exists()) {
                System.err.println("El archivo no existe.");
                return;
            }

            BufferedReader br = new BufferedReader(new FileReader(file));

            String line;
            int dimension = 0;
            String palabra = "";
            char[][] matrix = null;
            int currentLine = 0;

            while ((line = br.readLine()) != null) {
                if (currentLine == 0) {
                    dimension = Integer.parseInt(line);
                } else if (currentLine == 1) {
                    palabra = line;
                } else {
                    if (matrix == null) {
                        matrix = new char[dimension][dimension];
                    }
                    String rowWithoutSpaces = line.replaceAll(" ", "");
                    for (int i = 0; i < dimension; i++) {
                        matrix[currentLine - 2][i] = rowWithoutSpaces.charAt(i);
                    }
                }

                currentLine++;
            }

            br.close();

            System.out.println("\n\n" + palabra);

            // H E B R A S
            long timehebras = System.currentTimeMillis();
            Thread hebra = new Hebras(matrix, palabra, 0, dimension, 0, dimension, dimension, timehebras);
            hebra.start();
            try {
                hebra.join();
            } catch (InterruptedException e) {
            }

            // F O R K S
            long timeforks = System.currentTimeMillis();
            Forks fork = new Forks(matrix, palabra, 0, dimension, 0, dimension, dimension, timeforks);
            ForkJoinPool pool = new ForkJoinPool();
            pool.invoke(fork);

            // N A D A
            long timenada = System.currentTimeMillis();
            Nada nada = new Nada(matrix, palabra, dimension, timenada);
            nada.main();

        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
