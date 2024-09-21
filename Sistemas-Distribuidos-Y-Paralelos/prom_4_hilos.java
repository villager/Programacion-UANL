import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class main {
    public static int NUM_HILOS = 4;
    public static void main(String[] args) {
        String archivo = "numeros.txt";
        int[] numeros = leerNumerosDesdeArchivo(archivo);
        
        if (numeros == null || numeros.length == 0) {
            System.out.println("No se pudo leer el archivo o está vacío.");
            return;
        }

        int tamañoSegmento = numeros.length / NUM_HILOS;
        PromedioHilo[] hilos = new PromedioHilo[NUM_HILOS];
        double promedioTotal = 0;

        for (int i = 0; i < NUM_HILOS; i++) {
            int inicio = i * tamañoSegmento;
            int fin = (i == NUM_HILOS - 1) ? numeros.length : inicio + tamañoSegmento;
            hilos[i] = new PromedioHilo(numeros, inicio, fin);
            hilos[i].start();
        }

        try {
            for (PromedioHilo hilo : hilos) {
                hilo.join();
                promedioTotal += hilo.getPromedio();
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        promedioTotal /= NUM_HILOS;
        System.out.println("El promedio total es: " + promedioTotal);
    }

    private static int[] leerNumerosDesdeArchivo(String archivo) {
        try (BufferedReader reader = new BufferedReader(new FileReader(archivo))) {
            String linea = reader.readLine();
            if (linea != null) {
                String[] partes = linea.split(",");
                int[] numeros = new int[partes.length];
                for (int i = 0; i < partes.length; i++) {
                    numeros[i] = Integer.parseInt(partes[i]);
                }
                return numeros;
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }
}

class PromedioHilo extends Thread {
    public int[] numeros;
    public int inicio;
    public int fin;
    private double promedio;

    public PromedioHilo(int[] numeros, int inicio, int fin) {
        this.numeros = numeros;
        this.inicio = inicio;
        this.fin = fin;
    }

    @Override
    public void run() {
        long suma = 0;
        for (int i = inicio; i < fin; i++) {
            suma += numeros[i];
        }
        promedio = (double) suma / (fin - inicio);
    }

    public double getPromedio() {
        return promedio;
    }
}