/**
 * ============================================================================
 * ASIGNATURA: TECNOLOGÍA DE OBJETOS
 * PRÁCTICA 03: C++ Herencia, Polimorfismo, Qt
 * ESTUDIANTE: Franco Jesus Cahua Soto
 * ARCHIVO: ejercicio3.cpp
 * DESCRIPCIÓN: Implementación de un ARBOL DE EXPRESIONES BINARIO (Binary
 *              Expression Tree, BET) con clases y punteros, capaz de resolver
 *              operaciones de SUMA (+) y MULTIPLICACIÓN (*).
 *
 *                  Nodo (abstracta)
 *                   /            \
 *              Hoja(valor)   NodoOperacion(op, izq, der)
 *
 *              - `Nodo` define la interfaz comun mediante funciones virtuales
 *                puras (patron Template Method): evaluar() e|impresion en los
 *                tres ordenes de recorrido (prefijo / infijo / postfijo).
 *              - `Hoja` es una hoja terminal que almacena un literal.
 *              - `NodoOperacion` es un nodo interno que referencia dos hijos
 *                mediante punteros, contendo la tabla de evaluar() que aplica
 *                la precedencia aritmetica (+ antes que * segun el arbol).
 *              - `ConvertidorPostfijo` aplica el algoritmo del tren chorizo
 *                (shunting-yard) para pasar de notacion infija a postfija.
 *              - `ConstructorBET` apila los tokens postfijos y arma el arbol,
 *                devolviendo la raiz para su evaluacion polimorfica.
 *              - La liberacion es recursiva (delete sobre los hijos) que es
 *                justamente el patron que reemplaza std::unique_ptr en C++11.
 * ============================================================================
 */

#include <algorithm>
#include <cctype>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

// ============================================================================
// CLASE BASE ABSTRACTA: Nodo
// ============================================================================
class Nodo {
public:
    virtual ~Nodo() = default;

    // --- INTERFAZ PURAMENTE VIRTUAL DEL ARBOL DE EXPRESIONES ---
    virtual double evaluar() const = 0;
    virtual bool esHoja() const = 0;
    virtual char simbolo() const = 0;
    virtual void imprimirInorden(std::ostream& out, bool paren) const = 0;
    virtual void imprimirPreorden(std::ostream& out) const = 0;
    virtual void imprimirPostorden(std::ostream& out) const = 0;

    // --- METODOS VIRTUALES CON IMPLEMENTACION POR DEFECTO ---
    virtual int contarNodos() const { return 1; }
    virtual int altura() const { return 1; }

    // --- METODO VIRTUAL PURO: liberacion recursiva del subarbol ---
    virtual void liberar() = 0;

    // --- Tabla de precedencia aritmetica compartida por toda la jerarquia ---
    static int precedencia(char op) {
        if (op == '*') return 2;
        if (op == '+') return 1;
        return 0;
    }

    // --- Metodo NO virtual: recorrido en orden de nivel (BFS) ---
    void niveles() const {
        std::cout << "  raiz = primer token del recorrido prefijo\n";
        imprimirPreorden(std::cout);
    }
};

// ============================================================================
// NODO TERMINAL: Hoja con un literal numerico
// ============================================================================
class Hoja : public Nodo {
private:
    double valor;

public:
    explicit Hoja(double v) : valor(v) {}

    double evaluar() const override { return valor; }
    bool esHoja() const override { return true; }
    char simbolo() const override { return '\0'; }

    // Una hoja nunca necesita parentizacion: son literales atomicos.
    void imprimirInorden(std::ostream& out, bool) const override { out << valor; }
    void imprimirPreorden(std::ostream& out) const override { out << valor; }
    void imprimirPostorden(std::ostream& out) const override { out << valor; }

    void liberar() override { delete this; }

    double getValor() const { return valor; }
};

// ============================================================================
// NODO INTERNO: operacion binaria con dos hijos
// ============================================================================
class NodoOperacion : public Nodo {
private:
    char operador;
    Nodo* izq;
    Nodo* der;

public:
    NodoOperacion(char op, Nodo* izquierdo, Nodo* derecho)
        : operador(op), izq(izquierdo), der(derecho) {}

    // Nucleo del arbol de expresiones: aplica la operacion a ambos hijos.
    double evaluar() const override {
        const double a = izq->evaluar();
        const double b = der->evaluar();
        switch (operador) {
            case '+': return a + b;
            case '*': return a * b;
            default:
                std::cerr << "ERROR: operador no soportado '" << operador << "'\n";
                return std::numeric_limits<double>::quiet_NaN();
        }
    }

    bool esHoja() const override { return false; }
    char simbolo() const override { return operador; }

    // Infijo: "(3 + 4) * 5". Un hijo se parentiza SOLO si su operador tiene
    // menor precedencia que el del nodo actual; de otro modo el par de
    // parentesis seria redundante y la expression ambigua.
    void imprimirInorden(std::ostream& out, bool paren) const override {
        if (paren) out << "(";
        izq->imprimirInorden(out, precedencia(izq->simbolo()) < precedencia(operador));
        out << " " << operador << " ";
        der->imprimirInorden(out, precedencia(der->simbolo()) < precedencia(operador));
        if (paren) out << ")";
    }

    // Prefijo: "(+ 3 (* 4 5))"
    void imprimirPreorden(std::ostream& out) const override {
        out << "(" << operador << " ";
        izq->imprimirPreorden(out);
        out << " ";
        der->imprimirPreorden(out);
        out << ")";
    }

    // Postfijo: "3 4 5 * +"
    void imprimirPostorden(std::ostream& out) const override {
        izq->imprimirPostorden(out);
        out << " ";
        der->imprimirPostorden(out);
        out << " " << operador;
    }

    int contarNodos() const override {
        return 1 + izq->contarNodos() + der->contarNodos();
    }

    int altura() const override {
        return 1 + std::max(izq->altura(), der->altura());
    }

    // Liberacion en profundidad: primero los subarboles, luego el nodo.
    void liberar() override {
        izq->liberar();
        der->liberar();
        delete this;
    }
};

// ============================================================================
// MODULO 1: Conversion de notacion infija a postfija (Shunting-Yard)
// ============================================================================
class ConvertidorPostfijo {
private:
    std::vector<std::string> pila;

    static int precedencia(const std::string& token) {
        if (token == "*") return 2;
        if (token == "+") return 1;
        return 0;
    }

public:
    std::vector<std::string> convertir(const std::string& infija) {
        pila.clear();
        std::vector<std::string> salida;
        std::string numero;

        auto volcarNumero = [&]() {
            if (!numero.empty()) {
                salida.push_back(numero);
                numero.clear();
            }
        };

        for (char c : infija) {
            if (c == ' ') continue;

            if (isdigit(static_cast<unsigned char>(c)) || c == '.') {
                numero += c;                       // se acumula el literal
            } else if (c == '(') {
                volcarNumero();
                pila.push_back("(");               // marca de apertura
            } else if (c == ')') {
                volcarNumero();
                while (!pila.empty() && pila.back() != "(") {
                    salida.push_back(pila.back());
                    pila.pop_back();
                }
                if (pila.empty()) {
                    throw std::runtime_error("Parentesis desbalanceados en la expresion");
                }
                pila.pop_back();                   // descarta el "("
            } else {
                // El dominio del arbol se limita a SUMA y MULTIPLICACION.
                if (c != '+' && c != '*') {
                    throw std::runtime_error(
                        std::string("Operador no soportado '") + c +
                        " (solo suma y producto)");
                }
                volcarNumero();
                // Mayor o igual precedencia garantiza asociatividad izquierda.
                while (!pila.empty() && pila.back() != "(" &&
                       precedencia(pila.back()) >= precedencia(std::string(1, c))) {
                    salida.push_back(pila.back());
                    pila.pop_back();
                }
                pila.push_back(std::string(1, c));
            }
        }

        volcarNumero();
        while (!pila.empty()) {
            if (pila.back() == "(")
                throw std::runtime_error("Parentesis sin cerrar en la expresion");
            salida.push_back(pila.back());
            pila.pop_back();
        }
        return salida;
    }
};

// ============================================================================
// MODULO 2: construccion del arbol a partir de la expresion postfija
// ============================================================================
class ConstructorBET {
public:
    // Devuelve la raiz del arbol (nullptr si la expresion es invalida).
    Nodo* construir(const std::vector<std::string>& postfija) {
        std::stack<Nodo*> pila;
        try {
            for (const std::string& token : postfija) {
                if (token != "+" && token != "*") {
                    pila.push(new Hoja(std::stod(token)));
                } else {
                    if (pila.size() < 2)
                        throw std::runtime_error("Expresion postfija mal formada");
                    Nodo* der = pila.top(); pila.pop();
                    Nodo* izq = pila.top(); pila.pop();
                    pila.push(new NodoOperacion(token[0], izq, der));
                }
            }
            if (pila.size() != 1)
                throw std::runtime_error("La expresion postfija no reduce a una raiz unica");
            return pila.top();
        } catch (...) {
            // SEGURIDAD DE EXCEPCIONES: los nodos ya apilados son subarboles
            // disjuntos, por lo que basta con liberar cada uno. Sin este
            // drenaje la expresion `3+*5` filtraria sus 3 nodos intermedios.
            // (Con std::unique_ptr este drenaje seria innecesario: el destructor
            //  de la pila resolveria solo. Véase el cuestionario del informe.)
            while (!pila.empty()) {
                pila.top()->liberar();
                pila.pop();
            }
            throw;
        }
    }
};

// ============================================================================
// MODULO 3: evaluacion comparativa contra un evaluador de referencia
// ============================================================================
double evaluarReferencia(const std::string& infija) {
    // Se recorre el BET en postfijo con una pila de reales: el metodo clasico
    // de evaluacion de expresiones, equivalente al arbol pero iterativo.
    std::vector<std::string> post;
    ConvertidorPostfijo conv;
    post = conv.convertir(infija);
    std::stack<double> valores;
    for (const std::string& t : post) {
        if (t != "+" && t != "*") {
            valores.push(std::stod(t));
        } else {
            const double b = valores.top(); valores.pop();
            const double a = valores.top(); valores.pop();
            valores.push(t == "+" ? a + b : a * b);
        }
    }
    return valores.top();
}

// ============================================================================
// PROGRAMA PRINCIPAL
// ============================================================================
int main() {
    std::cout << "=========================================================\n";
    std::cout << "  TECNOLOGÍA DE OBJETOS - LAB 03 | EJERCICIO 03\n";
    std::cout << "  ARBOL DE EXPRESIONES BINARIO (suma y multiplicacion)\n";
    std::cout << "  Estudiante: Franco Jesus Cahua Soto\n";
    std::cout << "=========================================================\n\n";

    ConvertidorPostfijo conversor;
    ConstructorBET constructor;

    std::vector<std::string> pruebas = {
        "3+4*5",                  // precedencia: 3 + (4*5)
        "2*3+4*5",                // precedencia por ambos lados
        "3+4*5+1*10",             // suma de productos
        "(3+4)*5",                // parentesis fuerzan la precedencia
        "2*(3+4)*(5+6)",          // producto de dos sumas
        "100*2+50*3+25",          // suma de productos extensos
        "7*8*9",                  // producto multiple
        "1+2*3+4*5*6"             // caso compuesto
    };

    std::cout << ">>> 1. CONVERSION INFIJA -> POSTFIJA (algoritmo Shunting-Yard)\n";
    for (const std::string& e : pruebas) {
        std::vector<std::string> post = conversor.convertir(e);
        std::cout << "  " << std::left << std::setw(18) << e << " -> ";
        for (size_t i = 0; i < post.size(); ++i) {
            std::cout << post[i] << (i + 1 < post.size() ? " " : "");
        }
        std::cout << "\n";
    }

    std::cout << "\n>>> 2. CONSTRUCCION, RECORRIDOS Y EVALUACION DEL ARBOL\n";
    for (const std::string& e : pruebas) {
        Nodo* raiz = nullptr;
        try {
            raiz = constructor.construir(conversor.convertir(e));
        } catch (const std::exception& ex) {
            std::cout << "  Expresion invalida: " << ex.what() << "\n";
            continue;
        }

        std::ostringstream pre, inf, pos;
        raiz->imprimirPreorden(pre);
        raiz->imprimirInorden(inf, true);
        raiz->imprimirPostorden(pos);

        const double resultado = raiz->evaluar();
        const double referencia = evaluarReferencia(e);

        std::cout << "\n  Expresion infija : " << e << "\n"
                  << "  Prefijo  (raiz)  : " << pre.str() << "\n"
                  << "  Infijo  (legible): " << inf.str() << "\n"
                  << "  Postfijo (RPN)   : " << pos.str() << "\n"
                  << "  Nodos / Altura   : " << raiz->contarNodos() << " nodos, "
                  << raiz->altura() << " niveles\n"
                  << "  EVALUACION       : " << std::fixed << std::setprecision(4)
                  << resultado << "\n"
                  << "  Referencia       : " << referencia << "   ["
                  << (std::fabs(resultado - referencia) < 1e-9 ? "COINCIDE" : "DIFIERE")
                  << "]\n";

        raiz->liberar();
    }

    // --- Propiedad distributiva: verificacion estructural del arbol ---
    std::cout << "\n>>> 3. PROPIEDAD DISTRIBUTIVA: (a+b)*(c+d) = ac+ad+bc+bd\n";
    const std::string izquierda = "(2+3)*(4+5)";
    const std::string derecha = "2*4+2*5+3*4+3*5";
    Nodo* raizIzq = constructor.construir(conversor.convertir(izquierda));
    Nodo* raizDer = constructor.construir(conversor.convertir(derecha));
    std::ostringstream infIzq, infDer;
    raizIzq->imprimirInorden(infIzq, true);
    raizDer->imprimirInorden(infDer, true);
    std::cout << "  Forma factorizada : " << infIzq.str() << " = " << raizIzq->evaluar() << "\n"
              << "  Forma expandida   : " << infDer.str() << " = " << raizDer->evaluar() << "\n"
              << "  Nodos (factorizada / expandida): " << raizIzq->contarNodos()
              << " / " << raizDer->contarNodos() << "\n"
              << "  Conclusion: mismo valor, distinta complejidad estructural.\n";
    raizIzq->liberar();
    raizDer->liberar();

    // --- Manejo de expresiones invalidas ---
    std::cout << "\n>>> 4. VALIDACION DE ENTRADAS (lanzamiento de excepciones)\n";
    const std::vector<std::string> invalidas = { "(3+4*5", "3+4)", "3+*5", "10-4" };
    for (const std::string& e : invalidas) {
        Nodo* raiz = nullptr;
        try {
            raiz = constructor.construir(conversor.convertir(e));
            std::cout << "  " << std::left << std::setw(10) << e
                      << " -> ACEPTADA (valor " << raiz->evaluar() << ")\n";
            raiz->liberar();
        } catch (const std::exception& ex) {
            std::cout << "  " << std::left << std::setw(10) << e
                      << " -> RECHAZADA: " << ex.what() << "\n";
        }
    }

    // --- Demostracion del operador no soportado ---
    std::cout << "\n>>> 5. NODO CON OPERADOR FUERA DEL DOMINIO {+, *}\n";
    {
        Nodo* raizPrueba = new NodoOperacion('-',
                                new NodoOperacion('*', new Hoja(6), new Hoja(7)),
                                new Hoja(2));
        std::ostringstream inf;
        raizPrueba->imprimirInorden(inf, true);
        std::cout << "  Arbol : " << inf.str() << "\n"
                  << "  evaluate() invoca la rama `default` y reporta NAN:\n  -> "
                  << raizPrueba->evaluar() << "\n";
        raizPrueba->liberar();
    }

    std::cout << "\n>>> 6. LIBERACION RECURSIVA: sin fugas de memoria.\n";
    return 0;
}
