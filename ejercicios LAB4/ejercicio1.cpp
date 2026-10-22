// Ejercicio 1: contar los numeros primos menores de 1000 en un vector
// de enteros generados de forma aleatoria, usando std::count_if y lambdas.
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

int main()
{
    // Semilla fija para que la salida sea reproducible en el informe.
    std::mt19937 generador(2026);
    std::uniform_int_distribution<int> distribucion(1, 2000);

    std::vector<int> numeros(50);
    std::generate(numeros.begin(), numeros.end(),
                  [&]() { return distribucion(generador); });

    std::cout << "Vector generado (" << numeros.size() << " elementos):\n";
    for (int n : numeros) std::cout << n << ' ';
    std::cout << "\n\n";

    // Lambda que decide si un entero es primo (division de prueba hasta sqrt(n)).
    auto esPrimo = [](int n) {
        if (n < 2) return false;
        for (int d = 2; d * d <= n; ++d)
            if (n % d == 0) return false;
        return true;
    };

    // Lambda con el criterio completo del enunciado: primo Y menor de 1000.
    // 'esPrimo' se captura por copia ([=]).
    int cantidad = static_cast<int>(std::count_if(
        numeros.begin(), numeros.end(),
        [=](int n) { return n < 1000 && esPrimo(n); }));

    std::cout << "Primos menores de 1000 encontrados: " << cantidad << '\n';

    std::cout << "Primos (menores de 1000): ";
    std::for_each(numeros.begin(), numeros.end(), [&](int n) {
        if (n < 1000 && esPrimo(n)) std::cout << n << ' ';
    });
    std::cout << '\n';

    // Contraste: primos del vector sin el limite de 1000.
    int primosTotales = static_cast<int>(
        std::count_if(numeros.begin(), numeros.end(), esPrimo));
    std::cout << "Primos en todo el vector (sin limite): " << primosTotales << '\n';
    return 0;
}
