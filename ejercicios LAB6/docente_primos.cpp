// Ejemplo c) del docente: primos en [1, 100000], secuencial vs. paralelo
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <algorithm>

bool esPrimo(int n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (int d = 3; (long long)d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

int contarSecuencial(int ini, int fin) {
    int c = 0;
    for (int i = ini; i <= fin; ++i) c += esPrimo(i);
    return c;
}

// Funcion para procesamiento paralelo: cada hilo escribe solo su posicion
void contarRango(int ini, int fin, int* resultado) {
    *resultado = contarSecuencial(ini, fin);
}

int main() {
    const int MAXN = 100000;
    using reloj = std::chrono::steady_clock;

    auto t0 = reloj::now();
    int totalSec = contarSecuencial(1, MAXN);
    auto t1 = reloj::now();

    unsigned nh = std::max(2u, std::thread::hardware_concurrency());
    std::vector<std::thread> hilos;
    std::vector<int> parciales(nh, 0);
    int paso = MAXN / nh;
    auto t2 = reloj::now();
    for (unsigned i = 0; i < nh; ++i) {
        int ini = i * paso + 1;
        int fin = (i == nh - 1) ? MAXN : (i + 1) * paso;
        hilos.emplace_back(contarRango, ini, fin, &parciales[i]);
    }
    for (auto& h : hilos) h.join();
    auto t3 = reloj::now();
    int totalPar = 0;
    for (int p : parciales) totalPar += p;

    auto us = [](auto a, auto b) {
        return std::chrono::duration<double, std::micro>(b - a).count();
    };
    std::cout << "Hilos usados        : " << nh << "\n";
    std::cout << "Primos (secuencial) : " << totalSec << "  en " << us(t0, t1) << " us\n";
    std::cout << "Primos (paralelo)   : " << totalPar << "  en " << us(t2, t3) << " us\n";
    std::cout << "Resultados iguales  : " << (totalSec == totalPar ? "si" : "no") << "\n";
    return 0;
}
