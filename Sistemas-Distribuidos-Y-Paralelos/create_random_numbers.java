import java.io.FileWriter;
import java.io.IOException;
import java.util.Random;

public class GenerarArchivo {
    public static void main(String[] args) {
        int cantidadNumeros = 100000;
        Random random = new Random();
        
        try (FileWriter writer = new FileWriter("numeros.txt")) {
            for (int i = 0; i < cantidadNumeros; i++) {
                int numero = random.nextInt(1000); // Número aleatorio entre 0 y 999
                writer.write(numero + (i < cantidadNumeros - 1 ? "," : ""));
            }
            System.out.println("Archivo generado con éxito.");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
