#include <iostream>
#include <chrono>
using namespace std;

int main() {
    // Marcar el tiempo de inicio
    auto inicio = chrono::high_resolution_clock::now();

    // Bloque de código a medir
    for (int i = 0; i < 1; ++i) {
        cout << "hola mundo"<< "\n";
    }

    // Marcar el tiempo de finalización
    auto fin = chrono::high_resolution_clock::now();

    // Calcular duración
    chrono::duration<double, milli> duracion = fin - inicio;
    cout << "Tiempo transcurrido: " << duracion.count() << " ms" << endl;

    return 0;
}
