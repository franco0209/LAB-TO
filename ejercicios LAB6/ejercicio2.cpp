// Ejercicio 2: distancias Manhattan y Euclides con hilos, leyendo un CSV
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <thread>
#include <functional>
#include <cmath>
#include <iomanip>
#include <algorithm>

struct Fila {
    std::string origen, destino;
    double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    double euclidea_csv = 0, manhattan_csv = 0;   // valores del archivo (verificacion)
    double euclidea = 0, manhattan = 0;           // valores calculados por los hilos
};

bool leerCSV(const std::string& ruta, std::vector<Fila>& filas) {
    std::ifstream f(ruta);
    if (!f) return false;
    std::string linea;
    std::getline(f, linea);                  // encabezado
    while (std::getline(f, linea)) {
        if (linea.empty()) continue;
        std::stringstream ss(linea);
        std::string c[8];
        for (int i = 0; i < 8; ++i) std::getline(ss, c[i], ',');
        Fila r;
        r.origen = c[0]; r.destino = c[1];
        r.x1 = std::stod(c[2]); r.y1 = std::stod(c[3]);
        r.x2 = std::stod(c[4]); r.y2 = std::stod(c[5]);
        r.euclidea_csv = std::stod(c[6]); r.manhattan_csv = std::stod(c[7]);
        filas.push_back(r);
    }
    return true;
}

// Cada hilo procesa un rango disjunto [ini, fin): no hay condicion de carrera.
void calcular(std::vector<Fila>& filas, size_t ini, size_t fin) {
    for (size_t i = ini; i < fin; ++i) {
        Fila& r = filas[i];
        double dx = r.x2 - r.x1, dy = r.y2 - r.y1;
        r.euclidea  = std::sqrt(dx * dx + dy * dy);
        r.manhattan = std::fabs(dx) + std::fabs(dy);
    }
}

int main(int argc, char** argv) {
    std::string ruta = argc > 1 ? argv[1] : "distancias_manhattan_euclides.csv";
    std::vector<Fila> filas;
    if (!leerCSV(ruta, filas)) { std::cerr << "No se pudo abrir " << ruta << "\n"; return 1; }

    size_t n = filas.size();
    size_t nh = std::max<size_t>(1, std::min<size_t>(n, std::thread::hardware_concurrency()));
    std::vector<std::thread> hilos;
    size_t bloque = (n + nh - 1) / nh;
    for (size_t t = 0; t < nh; ++t) {
        size_t ini = t * bloque, fin = std::min(n, ini + bloque);
        if (ini < fin) hilos.emplace_back(calcular, std::ref(filas), ini, fin);
    }
    for (auto& h : hilos) h.join();

    std::cout << "Registros: " << n << " | hilos: " << hilos.size() << "\n\n";
    std::cout << std::left << std::setw(9) << "Origen" << std::setw(9) << "Destino"
              << std::right << std::setw(11) << "Euclides" << std::setw(11) << "Manhattan"
              << std::setw(12) << "Eucl.(CSV)" << std::setw(12) << "Manh.(CSV)" << "\n";
    std::cout << std::fixed << std::setprecision(4);
    for (size_t i = 0; i < std::min<size_t>(5, n); ++i) {
        const Fila& r = filas[i];
        std::cout << std::left << std::setw(9) << r.origen << std::setw(9) << r.destino
                  << std::right << std::setw(11) << r.euclidea << std::setw(11) << r.manhattan
                  << std::setw(12) << r.euclidea_csv << std::setw(12) << r.manhattan_csv << "\n";
    }
    bool ok = true;
    for (const Fila& r : filas)
        if (std::fabs(r.euclidea - r.euclidea_csv) > 1e-3 ||
            std::fabs(r.manhattan - r.manhattan_csv) > 1e-9) ok = false;
    std::cout << "\nCoincide con el CSV: " << (ok ? "si" : "no") << "\n";
    return 0;
}
