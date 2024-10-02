#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

const int MAX = 20; // Tamaño máximo de la tabla Simplex

struct Simplex {
    unsignedint numVariables, numRestricciones;
    double tabla[MAX][MAX]; // Tabla simplex
    int dirRestriccion[MAX]; // Donde guardar la direccion de las restricciones
    int base[MAX];          // Variables básicas
    bool multiple, ilimitada, noFactible;

    Simplex(int var, int restr) {
        numVariables = var;
        numRestricciones = restr;
        multiple = false;
        ilimitada = false;
        noFactible = false;
        // Inicializa la tabla y la base de variables básicas
        for (int i = 0; i < MAX; i++) {
            for (int j = 0; j < MAX; j++) {
                tabla[i][j] = 0;
            }
            base[i] = 0;
        }
    }

    void imprimirTabla() {
        cout << fixed << setprecision(2);
        cout << "\nTabla Simplex:\n";
        for (int i = 0; i <= numRestricciones; i++) {
            for (int j = 0; j <= numVariables + numRestricciones; j++) {
                cout << setw(8) << tabla[i][j] << " ";
            }
            cout << "\n";
        }
    }

    bool solucionOptima() {
        for (int j = 1; j <= numVariables + numRestricciones; j++) {
            if (tabla[0][j] < 0) {
                return false; // Todavía no es óptima
            }
        }
        return true; // Si todas son >= 0 es óptima
    }

    int columnaEntrada() {
        int col = 1;
        for (int j = 2; j <= numVariables + numRestricciones; j++) {
            if (tabla[0][j] < tabla[0][col]) {
                col = j;
            }
        }
        return col;
    }

    int filaSalida(int col) {
        int fila = -1;
        double minRatio = 1e9;

        for (int i = 1; i <= numRestricciones; i++) {
            if (tabla[i][col] > 0) {
                double ratio = tabla[i][0] / tabla[i][col];
                if (ratio < minRatio) {
                    minRatio = ratio;
                    fila = i;
                }
            }
        }

        if (fila == -1) {
            ilimitada = true;
        }

        return fila;
    }

    void pivotear(int fila, int col) {
        double pivote = tabla[fila][col];
        for (int j = 0; j <= numVariables + numRestricciones; j++) {
            tabla[fila][j] /= pivote;
        }

        for (int i = 0; i <= numRestricciones; i++) {
            if (i != fila) {
                double factor = tabla[i][col];
                for (int j = 0; j <= numVariables + numRestricciones; j++) {
                    tabla[i][j] -= factor * tabla[fila][j];
                }
            }
        }

        base[fila] = col;
    }

    void resolverSimplex() {
        while (!solucionOptima()) {
            int col = columnaEntrada();
            int fila = filaSalida(col);

            if (ilimitada) {
                cout << "El problema tiene solución no acotada.\n";
                return;
            }

            pivotear(fila, col);
            imprimirTabla();
        }

        imprimirResultado();
    }

    void imprimirResultado() {
        if (noFactible) {
            cout << "El problema es no factible.\n";
            return;
        }

        cout << "\nSolución óptima encontrada:\n";
        for (int i = 1; i <= numRestricciones; i++) {
            if (base[i] <= numVariables) {
                cout << "x" << base[i] << " = " << tabla[i][0] << "\n";
            }
        }

        cout << "Valor de la función objetivo: " << -tabla[0][0] << "\n";

        if (solucionMultiple()) {
            cout << "El problema tiene soluciones múltiples. Una variable para entrar en la base sería: x" << columnaEntrada() << "\n";
        }
    }

    bool solucionMultiple() {
        for (int j = 1; j <= numVariables + numRestricciones; j++) {
            if (tabla[0][j] == 0 && base[j] == 0) {
                return true; // Hay solución múltiple
            }
        }
        return false;
    }
};

int main() {
    int numVariables, numRestricciones;
    cout << "Ingrese el número de variables y restricciones: ";
    cin >> numVariables >> numRestricciones;
    Simplex simplex(numVariables, numRestricciones);
    cout << "Ingrese los coeficientes de la función objetivo:\n";
    for (int i = 1; i <= numVariables; i++) {
        cin >> tabla[0][i];
        tabla[0][i] *= -1; // Para maximizar
    }
    cout << "Ingrese las restricciones:\n";
    for (int i = 1; i <= numRestricciones; i++) {
        cout << "Restricción " << i << ":\n";
        for (int j = 1; j <= numVariables; j++) {
            cin >> tabla[i][j];
        }
        cin >> tabla[i][0]; // Lado derecho (b)
        cout << "¿La restriccion es mayor que ≤ (digita 1) o ≥ (digita 2)";
        cin >> simplex.dirRestriccion[i];
    }
    // Variables holgura
    for (int i = 1; i <= numRestricciones; i++) {
        tabla[i][numVariables + i] = 1;
        base[i] = numVariables + i; // Inicialmente variables básicas son holgura
    }
    simplex.ingresarDatos();
    simplex.imprimirTabla();
    simplex.resolverSimplex();

    return 0;
}
