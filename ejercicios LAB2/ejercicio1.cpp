/**
 * ============================================================================
 * ASIGNATURA: TECNOLOGÍA DE OBJETOS
 * PRÁCTICA 02: Puntero en C++
 * ESTUDIANTE: Franco Jesus Cahua Soto
 * ARCHIVO: ejercicio1.cpp
 * DESCRIPCIÓN: Implementación de una calculadora modular basada en 3 clases:
 *              1. AnalizadorOperacion: Tokeniza y valida sintácticamente la
 *                 cadena de texto (máximo 6 operandos).
 *              2. AdministradorOperaciones: Núcleo de cálculo aritmético
 *                 (gestiona suma, resta, multiplicación y división).
 *              3. ProcesadorCalculadora: Orquestador y despachador de la
 *                 operación, coordinando análisis y ejecución.
 * ============================================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <iomanip>
#include <stdexcept>

// ============================================================================
// CLASE 1: ANALIZADOR DE LA OPERACIÓN (Lexer / Parser)
// ============================================================================
class AnalizadorOperacion {
private:
    std::string expresionOriginal;
    std::vector<double> operandos;
    std::vector<char> operadores;
    bool sintaxisValida;
    std::string mensajeError;

public:
    AnalizadorOperacion() : sintaxisValida(false), mensajeError("") {}

    bool analizar(const std::string& expresion) {
        expresionOriginal = expresion;
        operandos.clear();
        operadores.clear();
        sintaxisValida = true;
        mensajeError = "";

        if (expresion.empty()) {
            sintaxisValida = false;
            mensajeError = "La expresion ingresada esta vacia.";
            return false;
        }

        std::string bufferNum = "";
        bool esperandoOperando = true;

        for (size_t i = 0; i < expresion.length(); ++i) {
            char c = expresion[i];
            if (std::isspace(c)) continue;

            if (std::isdigit(c) || c == '.') {
                bufferNum += c;
                esperandoOperando = false;
            } else if (c == '+' || c == '-' || c == '*' || c == '/') {
                if (esperandoOperando) {
                    if (c == '-' && bufferNum.empty()) {
                        bufferNum += c;
                        continue;
                    } else {
                        sintaxisValida = false;
                        mensajeError = "Operador consecutivo o fuera de lugar en posicion " + std::to_string(i);
                        return false;
                    }
                }

                try {
                    operandos.push_back(std::stod(bufferNum));
                } catch (...) {
                    sintaxisValida = false;
                    mensajeError = "Error al convertir numero: '" + bufferNum + "'";
                    return false;
                }

                operadores.push_back(c);
                bufferNum = "";
                esperandoOperando = true;

                if (operandos.size() >= 6) {
                    sintaxisValida = false;
                    mensajeError = "Se ha superado el limite maximo permitido de 6 numeros a operar.";
                    return false;
                }
            } else {
                sintaxisValida = false;
                mensajeError = std::string("Caracter no valido detectado: '") + c + "'";
                return false;
            }
        }

        if (!bufferNum.empty() && bufferNum != "-") {
            try {
                operandos.push_back(std::stod(bufferNum));
            } catch (...) {
                sintaxisValida = false;
                mensajeError = "Error al convertir ultimo numero: '" + bufferNum + "'";
                return false;
            }
        } else {
            sintaxisValida = false;
            mensajeError = "La expresion termina inesperadamente con un operador.";
            return false;
        }

        if (operandos.size() > 6) {
            sintaxisValida = false;
            mensajeError = "Maximo 6 numeros permitidos. Se detectaron " + std::to_string(operandos.size());
            return false;
        }

        return sintaxisValida;
    }

    const std::vector<double>& getOperandos() const { return operandos; }
    const std::vector<char>& getOperadores() const { return operadores; }
    bool esValido() const { return sintaxisValida; }
    std::string getError() const { return mensajeError; }
    size_t getCantidadNumeros() const { return operandos.size(); }
};

// ============================================================================
// CLASE 2: ADMINISTRADOR DE OPERACIONES (Núcleo / Engine Aritmético)
// ============================================================================
class AdministradorOperaciones {
public:
    double sumar(double a, double b) const { return a + b; }
    double restar(double a, double b) const { return a - b; }
    double multiplicar(double a, double b) const { return a * b; }
    double dividir(double a, double b) const {
        if (b == 0.0) {
            throw std::runtime_error("Error matematico: Division por cero detectada.");
        }
        return a / b;
    }

    double ejecutarOperacion(char op, double a, double b) const {
        switch (op) {
            case '+': return sumar(a, b);
            case '-': return restar(a, b);
            case '*': return multiplicar(a, b);
            case '/': return dividir(a, b);
            default:
                throw std::invalid_argument(std::string("Operador desconocido: ") + op);
        }
    }
};

// ============================================================================
// CLASE 3: PROCESADOR DE LA CALCULADORA (Coordinador y Despachador)
// ============================================================================
class ProcesadorCalculadora {
private:
    AnalizadorOperacion analizador;
    AdministradorOperaciones administrador;

public:
    bool procesar(const std::string& expresion, double& resultadoFinal) {
        std::cout << "\n--------------------------------------------------------------\n";
        std::cout << ">>> Procesando expresion: \"" << expresion << "\"\n";

        if (!analizador.analizar(expresion)) {
            std::cout << "    [RECHAZADO] " << analizador.getError() << "\n";
            return false;
        }

        std::vector<double> ops = analizador.getOperandos();
        std::vector<char> opers = analizador.getOperadores();

        std::cout << "    [ANALISIS EXITOSO] Operandos detectados (" << ops.size() << "): [ ";
        for (double v : ops) std::cout << v << " ";
        std::cout << "]\n";

        std::cout << "    Operadores detectados (" << opers.size() << "): [ ";
        for (char op : opers) std::cout << op << " ";
        std::cout << "]\n";

        try {
            // Fase 1: Multiplicaciones y divisiones (Mayor Precedencia)
            for (size_t i = 0; i < opers.size(); ) {
                if (opers[i] == '*' || opers[i] == '/') {
                    double r = administrador.ejecutarOperacion(opers[i], ops[i], ops[i + 1]);
                    std::cout << "    Traza: " << ops[i] << " " << opers[i] << " " << ops[i + 1] << " = " << r << "\n";
                    ops[i] = r;
                    ops.erase(ops.begin() + i + 1);
                    opers.erase(opers.begin() + i);
                } else {
                    ++i;
                }
            }

            // Fase 2: Sumas y restas de izquierda a derecha
            while (!opers.empty()) {
                double r = administrador.ejecutarOperacion(opers[0], ops[0], ops[1]);
                std::cout << "    Traza: " << ops[0] << " " << opers[0] << " " << ops[1] << " = " << r << "\n";
                ops[0] = r;
                ops.erase(ops.begin() + 1);
                opers.erase(opers.begin());
            }

            resultadoFinal = ops[0];
            std::cout << "    [RESULTADO FINAL] = " << std::fixed << std::setprecision(4) << resultadoFinal << "\n";
            return true;

        } catch (const std::exception& e) {
            std::cout << "    [EXCEPCION] " << e.what() << "\n";
            return false;
        }
    }
};

int main() {
    std::cout << "=========================================================\n";
    std::cout << "  TECNOLOGÍA DE OBJETOS - LAB 02 | EJERCICIO 01\n";
    std::cout << "  Calculadora Modular con 3 Clases\n";
    std::cout << "  Estudiante: Franco Jesus Cahua Soto\n";
    std::cout << "=========================================================\n";

    ProcesadorCalculadora procesador;
    double resultado = 0.0;

    // Pruebas solicitadas en la guía de laboratorio:
    std::vector<std::string> pruebasGuia = {
        "10+37",
        "45+14-42",
        "1+2+3+4+5+6",
        "100-25+15-10+50",
        "10*5+20/4-2",
        "1+2+3+4+5+6+7" // Excede el límite de 6 números para probar validación
    };

    std::cout << "\n>>> EJECUTANDO BATERIA DE PRUEBAS DE LA GUIA:\n";
    for (const auto& exp : pruebasGuia) {
        procesador.procesar(exp, resultado);
    }

    return 0;
}
