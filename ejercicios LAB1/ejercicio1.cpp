/**
 * ============================================================================
 * ASIGNATURA: TECNOLOGÍA DE OBJETOS
 * PRÁCTICA 01: C++ estructuras, clases y objetos
 * ESTUDIANTE: Franco Jesus Cahua Soto
 * ARCHIVO: ejercicio1.cpp
 * DESCRIPCIÓN: Programa que solicita un número entero positivo N.
 *              Permite generar N números aleatorios entre 1 y N ('g' con srand)
 *              o ingresarlos manualmente ('i'). Luego ordena de mayor a menor
 *              utilizando el algoritmo de Quicksort descendente.
 * ============================================================================
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iomanip>

// Función para intercambiar dos elementos
void intercambiar(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

// Partición para Quicksort en orden descendente (mayor a menor)
int particionDescendente(std::vector<int>& arr, int bajo, int alto) {
    int pivote = arr[alto]; // Seleccionamos el último elemento como pivote
    int i = bajo - 1;

    for (int j = bajo; j < alto; ++j) {
        // En orden descendente, los elementos mayores van a la izquierda
        if (arr[j] >= pivote) {
            ++i;
            intercambiar(arr[i], arr[j]);
        }
    }
    intercambiar(arr[i + 1], arr[alto]);
    return i + 1;
}

// Algoritmo Quicksort recursivo descendente
void quicksortDescendente(std::vector<int>& arr, int bajo, int alto) {
    if (bajo < alto) {
        int pi = particionDescendente(arr, bajo, alto);
        quicksortDescendente(arr, bajo, pi - 1);
        quicksortDescendente(arr, pi + 1, alto);
    }
}

// Función auxiliar para imprimir el arreglo
void imprimirArreglo(const std::vector<int>& arr, const std::string& mensaje) {
    std::cout << "\n" << mensaje << " (" << arr.size() << " elementos):\n[ ";
    for (size_t i = 0; i < arr.size(); ++i) {
        std::cout << arr[i];
        if (i + 1 < arr.size()) std::cout << ", ";
    }
    std::cout << " ]\n";
}

int main() {
    std::cout << "=========================================================\n";
    std::cout << "  TECNOLOGÍA DE OBJETOS - LAB 01 | EJERCICIO 01\n";
    std::cout << "  Estudiante: Franco Jesus Cahua Soto\n";
    std::cout << "=========================================================\n\n";

    int N = 0;
    while (true) {
        std::cout << "Digite N (entero positivo): ";
        if (std::cin >> N && N > 0) {
            break;
        } else {
            std::cout << "[ERROR] Entrada invalida. Ingrese un entero estrictamente positivo.\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
    }

    char opcion = ' ';
    while (true) {
        std::cout << "Seleccione modo de entrada:\n";
        std::cout << "  'g' : Autogenerar N numeros aleatorios entre 1 y N (usando srand)\n";
        std::cout << "  'i' : Ingresar manualmente N numeros enteros\n";
        std::cout << "Opcion: ";
        std::cin >> opcion;
        opcion = std::tolower(opcion);

        if (opcion == 'g' || opcion == 'i') {
            break;
        } else {
            std::cout << "[ERROR] Opcion no reconocida. Ingrese 'g' o 'i'.\n\n";
        }
    }

    std::vector<int> numeros(N);

    if (opcion == 'g') {
        // Inicializar la semilla de números pseudoaleatorios con el tiempo actual
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        std::cout << "\nGenerando " << N << " numeros aleatorios en el intervalo [1, " << N << "]...\n";
        for (int i = 0; i < N; ++i) {
            numeros[i] = (std::rand() % N) + 1;
        }
    } else {
        std::cout << "\nIngrese los " << N << " numeros enteros separados por espacio o saltos de linea:\n";
        for (int i = 0; i < N; ++i) {
            std::cout << "  Elemento [" << (i + 1) << "]: ";
            while (!(std::cin >> numeros[i])) {
                std::cout << "  [ERROR] Ingrese un valor entero valido: ";
                std::cin.clear();
                std::cin.ignore(10000, '\n');
            }
        }
    }

    // Mostrar arreglo antes de ordenar
    imprimirArreglo(numeros, "Arreglo Original");

    // Ordenar de mayor a menor con Quicksort
    quicksortDescendente(numeros, 0, N - 1);

    // Mostrar arreglo ordenado
    imprimirArreglo(numeros, "Arreglo Ordenado (Mayor a Menor)");

    std::cout << "\nProceso finalizado exitosamente.\n";
    return 0;
}
