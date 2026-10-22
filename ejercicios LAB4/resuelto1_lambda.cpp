// Ejercicio resuelto 1 de la guia: funciones lambda.
// Version en C++ estandar (la guia usa qDebug() de Qt; aqui std::cout).
#include <algorithm>
#include <iostream>
#include <vector>

int main()
{
    // Lambda asignada a un puntero a funcion (no captura nada).
    int (*s)(int) = [](int valor) { return valor + 3; };
    int u = 4;
    std::cout << "s(" << u << ") = " << s(u) << '\n';

    // Lambda declarada con 'auto'.
    auto R = [](int valor) { return valor; };
    std::cout << "R(7) = " << R(7) << '\n';

    // Captura por valor [=] y por referencia [&].
    int base = 10;
    auto porValor = [=](int x) { return x + base; };   // base no se modifica
    auto porRef   = [&](int x) { base += x; return base; };
    std::cout << "porValor(5) = " << porValor(5) << '\n';
    std::cout << "porRef(5)   = " << porRef(5) << "  (base ahora vale " << base << ")\n";

    // Recorrer los elementos de un vector con una lambda.
    std::vector<int> v{1, 2, 3, 4, 5};
    std::cout << "Elementos: ";
    std::for_each(v.begin(), v.end(), [](int e) { std::cout << e << ' '; });
    std::cout << '\n';
    return 0;
}
