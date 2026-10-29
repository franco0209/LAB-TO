/**
 * ASIGNATURA: TECNOLOGIA DE OBJETOS
 * PRACTICA 05: C++ Templates, Qt / Widgets parte II
 * ESTUDIANTE: Franco Jesus Cahua Soto
 * ARCHIVO: ejercicio4.cpp
 * DESCRIPCION: Arbol binario de busqueda como clase plantilla Arbol<T>.
 *   Admite cualquier T comparable con < (enteros, flotantes, char, string).
 *   Operaciones: insertar, buscar, minimo, maximo, altura, tamanio,
 *   recorridos preorden / inorden / postorden y destructor recursivo.
 */
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

template <typename T>
class Arbol {
    struct Nodo {
        T dato;
        Nodo *izq, *der;
        explicit Nodo(const T& d) : dato(d), izq(nullptr), der(nullptr) {}
    };
    Nodo* raiz;
    int cantidad;

    static bool insertar(Nodo*& n, const T& v) {
        if (!n) { n = new Nodo(v); return true; }
        if (v < n->dato) return insertar(n->izq, v);
        if (n->dato < v) return insertar(n->der, v);
        return false;                      // duplicado: se ignora
    }
    static void destruir(Nodo* n) {
        if (!n) return;
        destruir(n->izq);
        destruir(n->der);
        delete n;
    }
    static int altura(const Nodo* n) {
        return n ? 1 + max(altura(n->izq), altura(n->der)) : 0;
    }
    static void pre(const Nodo* n)  { if (!n) return; cout << n->dato << " "; pre(n->izq);  pre(n->der); }
    static void in(const Nodo* n)   { if (!n) return; in(n->izq);  cout << n->dato << " "; in(n->der); }
    static void post(const Nodo* n) { if (!n) return; post(n->izq); post(n->der); cout << n->dato << " "; }

public:
    Arbol() : raiz(nullptr), cantidad(0) {}
    Arbol(const Arbol&) = delete;              // evita doble delete
    Arbol& operator=(const Arbol&) = delete;
    ~Arbol() { destruir(raiz); }

    bool insertar(const T& v) {
        bool ok = insertar(raiz, v);
        if (ok) ++cantidad;
        return ok;
    }
    bool buscar(const T& v) const {
        const Nodo* n = raiz;
        while (n) {
            if (v < n->dato) n = n->izq;
            else if (n->dato < v) n = n->der;
            else return true;
        }
        return false;
    }
    const T& minimo() const { const Nodo* n = raiz; while (n->izq) n = n->izq; return n->dato; }
    const T& maximo() const { const Nodo* n = raiz; while (n->der) n = n->der; return n->dato; }
    int altura() const { return altura(raiz); }
    int tamanio() const { return cantidad; }

    void preorden() const  { cout << "  preorden : "; pre(raiz);  cout << endl; }
    void inorden() const   { cout << "  inorden  : "; in(raiz);   cout << endl; }
    void postorden() const { cout << "  postorden: "; post(raiz); cout << endl; }
};

template <typename T>
void probar(const string& titulo, const T* datos, int n, const T& buscado, const T& ausente) {
    cout << "=== " << titulo << " ===" << endl;
    Arbol<T> a;
    for (int i = 0; i < n; ++i) a.insertar(datos[i]);
    a.preorden(); a.inorden(); a.postorden();
    cout << "  tamanio = " << a.tamanio() << ", altura = " << a.altura()
         << ", min = " << a.minimo() << ", max = " << a.maximo() << endl;
    cout << "  buscar(" << buscado << ") = " << (a.buscar(buscado) ? "si" : "no")
         << " | buscar(" << ausente << ") = " << (a.buscar(ausente) ? "si" : "no") << endl;
    cout << "  insertar duplicado de " << datos[0] << " -> "
         << (a.insertar(datos[0]) ? "insertado" : "rechazado") << endl << endl;
}

int main() {
    int enteros[] = {50, 30, 70, 20, 40, 60, 80, 30};
    double reales[] = {3.5, 1.25, 7.75, 2.0, 9.5};
    char letras[] = {'m', 'c', 'x', 'a', 'e', 'z'};
    string palabras[] = {"mango", "banana", "uva", "cereza", "kiwi"};

    probar<int>("Arbol<int>", enteros, 8, 40, 45);
    probar<double>("Arbol<double>", reales, 5, 2.0, 4.0);
    probar<char>("Arbol<char>", letras, 6, 'e', 'q');
    probar<string>("Arbol<string>", palabras, 5, string("kiwi"), string("pera"));
    return 0;
}
