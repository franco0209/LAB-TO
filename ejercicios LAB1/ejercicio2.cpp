/**
 * ============================================================================
 * ASIGNATURA: TECNOLOGÍA DE OBJETOS
 * PRÁCTICA 01: C++ estructuras, clases y objetos
 * ESTUDIANTE: Franco Jesus Cahua Soto
 * ARCHIVO: ejercicio2.cpp
 * DESCRIPCIÓN: Programa que lista estadísticas deportivas (Tabla de Posiciones
 *              de Fútbol - Torneo de Primera División).
 *              Parte 1: Presenta los datos ordenados usando 'std::setw' (iomanip).
 *              Parte 2: Reutiliza el código sustituyendo 'setw' por un manipulador
 *                       y formateador alternativo (especificadores C-style / padding
 *                       manual por stream).
 *              Incluye comparativa técnica: ¿Cuál ha servido mejor?
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdio>

struct EstadisticaEquipo {
    int posicion;
    std::string equipo;
    int pj;   // Partidos Jugados
    int pg;   // Partidos Ganados
    int pe;   // Partidos Empatados
    int pp;   // Partidos Perdidos
    int gf;   // Goles a Favor
    int gc;   // Goles en Contra
    int dg;   // Diferencia de Goles (gf - gc)
    int pts;  // Puntos totales (pg*3 + pe*1)
    double eficacia; // (pts / (pj * 3)) * 100

    EstadisticaEquipo(std::string eq, int p_pj, int p_pg, int p_pe, int p_pp, int p_gf, int p_gc)
        : posicion(0), equipo(eq), pj(p_pj), pg(p_pg), pe(p_pe), pp(p_pp),
          gf(p_gf), gc(p_gc), dg(p_gf - p_gc), pts(p_pg * 3 + p_pe) {
        eficacia = (pj > 0) ? (static_cast<double>(pts) / (pj * 3.0)) * 100.0 : 0.0;
    }
};

// Criterio de ordenación: primero por puntos desc, luego por diferencia de goles desc, luego por goles a favor
bool criterioOrdenacion(const EstadisticaEquipo& a, const EstadisticaEquipo& b) {
    if (a.pts != b.pts) return a.pts > b.pts;
    if (a.dg != b.dg) return a.dg > b.dg;
    return a.gf > b.gf;
}

// ============================================================================
// VERSIÓN 1: Formateo estricto utilizando 'std::setw', 'std::left', 'std::right'
// ============================================================================
void mostrarTablaConSetw(const std::vector<EstadisticaEquipo>& tabla) {
    std::cout << "\n========================================================================================\n";
    std::cout << "        TABLA DE POSICIONES - LIGA 1 PROFESIONAL (FORMATEO CON std::setw)\n";
    std::cout << "========================================================================================\n";

    // Encabezado de columnas
    std::cout << std::left  << std::setw(5)  << "POS"
              << std::left  << std::setw(22) << "EQUIPO"
              << std::right << std::setw(5)  << "PJ"
              << std::right << std::setw(5)  << "PG"
              << std::right << std::setw(5)  << "PE"
              << std::right << std::setw(5)  << "PP"
              << std::right << std::setw(6)  << "GF"
              << std::right << std::setw(6)  << "GC"
              << std::right << std::setw(6)  << "DG"
              << std::right << std::setw(6)  << "PTS"
              << std::right << std::setw(10) << "EFIC (%)\n";

    std::cout << std::string(88, '-') << "\n";

    // Filas de datos
    for (const auto& eq : tabla) {
        std::cout << std::left  << std::setw(5)  << eq.posicion
                  << std::left  << std::setw(22) << eq.equipo
                  << std::right << std::setw(5)  << eq.pj
                  << std::right << std::setw(5)  << eq.pg
                  << std::right << std::setw(5)  << eq.pe
                  << std::right << std::setw(5)  << eq.pp
                  << std::right << std::setw(6)  << eq.gf
                  << std::right << std::setw(6)  << eq.gc
                  << std::right << std::setw(6)  << (eq.dg > 0 ? "+" + std::to_string(eq.dg) : std::to_string(eq.dg))
                  << std::right << std::setw(6)  << eq.pts
                  << std::right << std::setw(9)  << std::fixed << std::setprecision(1) << eq.eficacia << "%\n";
    }
    std::cout << "========================================================================================\n";
}

// ============================================================================
// VERSIÓN 2: Reutilización de código reemplazando 'setw' por especificadores de
//            ancho y alineación por cadenas formateadas (printf / snprintf formatters)
// ============================================================================
void mostrarTablaConFormatoAlternativo(const std::vector<EstadisticaEquipo>& tabla) {
    std::cout << "\n========================================================================================\n";
    std::cout << "   TABLA DE POSICIONES - LIGA 1 PROFESIONAL (FORMATEO ALTERNATIVO CON PRINTF / SNPRINTF)\n";
    std::cout << "========================================================================================\n";

    // Encabezado
    std::printf("%-5s%-22s%5s%5s%5s%5s%6s%6s%6s%6s%10s\n",
                "POS", "EQUIPO", "PJ", "PG", "PE", "PP", "GF", "GC", "DG", "PTS", "EFIC (%)");
    std::cout << std::string(88, '=') << "\n";

    for (const auto& eq : tabla) {
        std::string dgStr = (eq.dg > 0) ? ("+" + std::to_string(eq.dg)) : std::to_string(eq.dg);
        std::printf("%-5d%-22s%5d%5d%5d%5d%6d%6d%6s%6d%9.1f%%\n",
                    eq.posicion,
                    eq.equipo.c_str(),
                    eq.pj, eq.pg, eq.pe, eq.pp,
                    eq.gf, eq.gc, dgStr.c_str(),
                    eq.pts, eq.eficacia);
    }
    std::cout << "========================================================================================\n";
}

void mostrarAnalisisComparativo() {
    std::cout << "\n========================================================================================\n";
    std::cout << "                EVALUACIÓN COMPARATIVA: ¿CUÁL HA SERVIDO MEJOR?\n";
    std::cout << "========================================================================================\n";
    std::cout << "1. Manipulador 'std::setw' (de <iomanip>):\n";
    std::cout << "   + Ventajas: Tipado seguro (type-safe), integracion natural con flujos C++ (std::cout),\n";
    std::cout << "     compatibilidad completa con tipos definidos por el usuario (sobrecarga operator<<).\n";
    std::cout << "   - Desventajas: Es efimero (se reinicia tras cada operacion de insercion), requiriendo\n";
    std::cout << "     escribir repetidamente 'std::setw(n)' para cada campo, tornando el codigo verboso.\n\n";

    std::cout << "2. Formateador alternativo (especificadores de ancho estilo printf / snprintf):\n";
    std::cout << "   + Ventajas: Sintesis y legibilidad en una sola cadena de plantilla (e.g. '%-22s%5d%6.1f%%').\n";
    std::cout << "     Facilita la definicion de anchos fijos y alineacion izquierda (-) o derecha sin repetir llamadas.\n";
    std::cout << "   - Desventajas: No es type-safe; si los tipos no coinciden con la mascara produce comportamientos\n";
    std::cout << "     indefinidos en compilacion/ejecucion clasica.\n\n";

    std::cout << "Conclusion del laboratorio:\n";
    std::cout << "   'std::setw' es preferible en proyectos C++ puros y orientados a objetos por su robustez,\n";
    std::cout << "   mientras que cadenas con mascara (estilo printf o std::format en C++20) ofrecen mayor\n";
    std::cout << "   claridad visual y concision al construir reportes tabulares complejos.\n";
    std::cout << "========================================================================================\n";
}

int main() {
    std::cout << "=========================================================\n";
    std::cout << "  TECNOLOGÍA DE OBJETOS - LAB 01 | EJERCICIO 02\n";
    std::cout << "  Estudiante: Franco Jesus Cahua Soto\n";
    std::cout << "=========================================================\n";

    // Dataset propuesto: Equipos del campeonato
    std::vector<EstadisticaEquipo> equipos = {
        EstadisticaEquipo("Universitario",       17, 12, 4, 1, 32, 10),
        EstadisticaEquipo("Alianza Lima",         17, 11, 4, 2, 28, 11),
        EstadisticaEquipo("Sporting Cristal",     17, 10, 4, 3, 35, 15),
        EstadisticaEquipo("FBC Melgar",           17,  9, 5, 3, 30, 16),
        EstadisticaEquipo("Cusco FC",             17,  9, 4, 4, 26, 20),
        EstadisticaEquipo("Cienciano",            17,  7, 5, 5, 22, 21),
        EstadisticaEquipo("ADT Tarma",            17,  6, 5, 6, 19, 20),
        EstadisticaEquipo("Atletico Grau",        17,  5, 7, 5, 18, 17),
        EstadisticaEquipo("Sport Huancayo",       17,  5, 4, 8, 17, 24),
        EstadisticaEquipo("Deportivo Garcilaso",   17,  4, 6, 7, 16, 22)
    };

    // Ordenar los equipos según desempeño deportivo
    std::sort(equipos.begin(), equipos.end(), criterioOrdenacion);

    // Asignar posición ordinal ordenada
    for (size_t i = 0; i < equipos.size(); ++i) {
        equipos[i].posicion = static_cast<int>(i + 1);
    }

    // Ejecutar presentación con std::setw
    mostrarTablaConSetw(equipos);

    // Ejecutar presentación con el manipulador alternativo reutilizado
    mostrarTablaConFormatoAlternativo(equipos);

    // Mostrar el análisis crítico y respuesta a la pregunta de la guía
    mostrarAnalisisComparativo();

    return 0;
}
