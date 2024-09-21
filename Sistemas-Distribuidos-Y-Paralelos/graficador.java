import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.data.category.DefaultCategoryDataset;

import javax.swing.*;
import java.io.*;
import java.util.*;
import java.awt.*;
import java.awt.event.*;

public class main {
    public static MainScreen ventana;
    public static void main(String[] args) {
        String archivo = "numeros.txt";
        int[] numeros = leerNumerosDesdeArchivo(archivo);

        if (numeros == null || numeros.length == 0) {
            ventana = new MainScreen("Error");
            ventana.ErrorMessage("No se pudo leer el archivo o está vacío.");
        } else {
            ventana = new MainScreen("Calcular el promedio de " + numeros.length + " números");
            ventana.EtiquetaPrincipal("Ingresa la cantidad de hilos que deseas utilizar");
            ventana.addInput();
            ventana.addSaveButton(numeros);
            ventana.addGraphButton(numeros.length);
        }
    }

    public static void usingThreads(int[] n, int threads) {
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
        totalTime = (System.nanoTime() - startTime);

        // Mostrar el resultado en la ventana
        ventana.mostrarResultado("El promedio total es: " + promedioTotal + 
                                  "\nEl tiempo de ejecución con " + threads + " hilos fue de: " + totalTime / 1e6 + " ms");
        writeFileCSV("results.csv", n.length, threads, totalTime / 1e6);
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

    private static void writeFileCSV(String archivo, int totalNumeros, int numHilos, double tiempoEjecucion) {
        try (BufferedWriter bw = new BufferedWriter(new FileWriter(archivo, true))) {
            File file = new File(archivo);
            // Si el archivo no existe, lo creamos y añadimos la cabecera
            if (!file.exists()) {
                bw.write("TotalNumeros,NumHilos,TiempoEjecucion(ms)\n");
            }
            bw.write(totalNumeros + "," + numHilos + "," + tiempoEjecucion + "\n");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static Map<Integer, Map<Integer, Double>> leerResultadosCSV(String archivo, int cantidadNumeros) {
        Map<Integer, Map<Integer, Double>> resultados = new HashMap<>();

        try (BufferedReader br = new BufferedReader(new FileReader(archivo))) {
            br.readLine();  // Lee la cabecera y la ignora

            String linea;
            while ((linea = br.readLine()) != null) {
                String[] valores = linea.split(",");
                int totalNumeros = Integer.parseInt(valores[0]);
                int numHilos = Integer.parseInt(valores[1]);
                double tiempoEjecucion = Double.parseDouble(valores[2]);

                // Filtrar por la cantidad de números
                if (totalNumeros == cantidadNumeros) {
                    resultados.putIfAbsent(numHilos, new HashMap<>());
                    Map<Integer, Double> hiloTiempoMap = resultados.get(numHilos);

                    if (!hiloTiempoMap.containsKey(numHilos) || tiempoEjecucion < hiloTiempoMap.get(numHilos)) {
                        hiloTiempoMap.put(numHilos, tiempoEjecucion);
                    }
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        return resultados;
    }
    public static void graficarResultados(Map<Integer, Map<Integer, Double>> resultados) {
    DefaultCategoryDataset dataset = new DefaultCategoryDataset();

    for (Map.Entry<Integer, Map<Integer, Double>> entry : resultados.entrySet()) {
        int numHilos = entry.getKey();
        double tiempoEjecucion = entry.getValue().values().iterator().next();
        dataset.addValue(tiempoEjecucion, "Tiempo (ms)", String.valueOf(numHilos));
    }
    if (dataset.getRowCount() == 0) {
        JOptionPane.showMessageDialog(null, "No se encontraron datos para graficar.");
        return;
    }

    JFreeChart lineChart = ChartFactory.createLineChart(
            "Resultados de Ejecución",
            "Número de Hilos",
            "Tiempo de Ejecución (ms)",
            dataset,
            PlotOrientation.VERTICAL,
            true, true, false);

    ChartPanel chartPanel = new ChartPanel(lineChart);
    chartPanel.setPreferredSize(new Dimension(800, 600));
    JFrame ventanaGrafico = new JFrame("Gráfica de Resultados");
    ventanaGrafico.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
    ventanaGrafico.setSize(800, 600);
    ventanaGrafico.add(chartPanel);
    ventanaGrafico.pack(); // Ajustar tamaño
        ventanaGrafico.setVisible(true);
    }

}

class MainScreen extends JFrame {
    private JPanel panel;
    private JLabel label;
    private JTextField text;
    private JButton saveButton;
    private JButton graphButton;
    private JLabel resultadoLabel;

    public MainScreen(String title) {
        setBounds(50, 50, 500, 500);
        setTitle(title);
        setSize(420, 420);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());
        panel = new JPanel();
        panel.setLayout(null); // Layout para posiciones absolutas
        this.getContentPane().add(panel);
        setVisible(true);

        // Crear la etiqueta para mostrar el resultado
        resultadoLabel = new JLabel();
        resultadoLabel.setBounds(30, 150, 350, 50); // Tamaño y posición de la etiqueta
        resultadoLabel.setFont(new Font("Arial", Font.PLAIN, 14));
        panel.add(resultadoLabel);
    }

    void ErrorMessage(String text) {
        JLabel etiqueta = new JLabel(text);
        etiqueta.setBounds(30, 10, 300, 30);
        etiqueta.setFont(new Font("Arial", Font.PLAIN, 15));
        panel.add(etiqueta);
        panel.revalidate();  // Actualiza la interfaz
        panel.repaint();     // Redibuja la interfaz
    }

    void EtiquetaPrincipal(String text) {
        JLabel etiqueta = new JLabel(text);
        etiqueta.setBounds(30, 10, 350, 30);
        etiqueta.setFont(new Font("Arial", Font.PLAIN, 15));
        panel.add(etiqueta);
    }

    void addInput() {
        text = new JTextField(20);  // Ancho del campo de texto
        text.setBounds(50, 50, 200, 30);
        panel.add(text);
    }

    void addSaveButton(int[] numeros) {
        saveButton = new JButton("Calcular Resultado");
        saveButton.setBounds(100, 100, 200, 40);

        // Añade listener para manejar el click del botón
        saveButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String inputText = text.getText();
                try {
                    int numHilos = Integer.parseInt(inputText);
                    if (numHilos > 0) {
                        main.usingThreads(numeros, numHilos);
                    } else {
                        ErrorMessage("Por favor, ingresa un número de hilos válido.");
                    }
                } catch (NumberFormatException ex) {
                    ErrorMessage("Ingresa un número válido de hilos.");
                }
            }
        });
        panel.add(saveButton);
        panel.revalidate();  
        panel.repaint();    
    }

    void addGraphButton(int numbers) {
        graphButton = new JButton("Graficar Resultados");
        graphButton.setBounds(100, 200, 200, 40);
        graphButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    Map<Integer, Map<Integer, Double>> resultados = main.leerResultadosCSV("results.csv", numbers);
                    if (!resultados.isEmpty()) {
                        main.graficarResultados(resultados);
                    } else {
                        ErrorMessage("No se encontraron resultados para la cantidad de números ingresada.");
                    }
                } catch (NumberFormatException ex) {
                    ErrorMessage("Ingresa un número válido.");
                }
            }
        });
        panel.add(graphButton);
        panel.revalidate();  
        panel.repaint();    
    }

    void mostrarResultado(String resultado) {
        resultadoLabel.setText("<html>" + resultado.replace("\n", "<br>") + "</html>");
        panel.revalidate();
        panel.repaint();    
    }
}

class PromedioHilo extends Thread {
    private int[] numeros;
    private int inicio;
    private int fin;
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