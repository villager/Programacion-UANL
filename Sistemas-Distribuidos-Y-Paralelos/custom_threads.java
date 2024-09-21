import java.util.Scanner;  // Import the Scanner class
import java.io.*;


/**
 * Archivo numberos2 contiene 10 millones de numeros
 * Archivo numberos contiene 100 000 numeros
 * @author aldair
 */
public class main {
    public static void main(String[] args) {
        String archivo = "numeros.txt";
        int[] numeros = leerNumerosDesdeArchivo(archivo);
        
        if (numeros == null || numeros.length == 0) {
            System.out.println("No se pudo leer el archivo o está vacío.");
            return;
        }
        System.out.println("Ingresa la cantidad de hilos a utilizar: ");
        Scanner threadsCount = new Scanner(System.in);  // Create a Scanner object
        int threadCount = Integer.parseInt(threadsCount.nextLine());
        usingThreads(numeros, threadCount);
    }
    private static void usingThreads(int[] n, int threads) {
        
        int tamañoSegmento = n.length / threads;
        PromedioHilo[] hilos = new PromedioHilo[threads];
        double promedioTotal = 0;
        long totalTime;
        long startTime = System.nanoTime();

        for (int i = 0; i < threads; i++) {
            int inicio = i * tamañoSegmento;
            int fin = (i == threads - 1) ? n.length : inicio + tamañoSegmento;
            hilos[i] = new PromedioHilo(n, inicio, fin);
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
        promedioTotal /= threads;
        totalTime = (System.nanoTime()-startTime); 
        System.out.println("El promedio total es: " + promedioTotal);
        System.out.println("El tiempo de ejecucion con "+ threads + " hilos fue de: " + totalTime/1e6 + "ms");
        writeFile("results.txt","Para "+ n.length + " numeros, usando " + threads + " hilos el tiempo de ejecucion fue de "+ totalTime/1e6 + " y con un promedio total por hilo es: " + promedioTotal + "\n" );
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
           // e.printStackTrace();
        }
        return null;
    }
    private static void writeFile(String archivo, String data) {
        BufferedWriter bw = null;
        FileWriter fw = null;
      try {
            File file = new File(archivo);
            // Si el archivo no existe, se crea!
        if (!file.exists()) {
            file.createNewFile();
        }
        // flag true, indica adjuntar información al archivo.
        fw = new FileWriter(file.getAbsoluteFile(), true);
        bw = new BufferedWriter(fw);
        bw.write(data);
    } catch (IOException e) {
        e.printStackTrace();
        } finally {
            try {
                if (bw != null)
                    bw.close();
                if (fw != null)
                    fw.close();
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        }
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

    public void getProm() {
        long suma = 0;
        for (int i = inicio; i < fin; i++) {
            suma += numeros[i];
        }
        promedio = (double) suma / (fin - inicio);
    }
    public double getPromedio() {
        this.getProm();
        return promedio;
    }
}