/**
 * ASIGNATURA: TECNOLOGIA DE OBJETOS
 * PRACTICA 05: C++ Templates, Qt / Widgets parte II
 * ESTUDIANTE: Franco Jesus Cahua Soto
 * ARCHIVO: ejercicio1.cpp
 * DESCRIPCION: Lista enlazada simple como clase plantilla ListaEnlazada<T>.
 *   Se instancia con int (secuencia de edades) y std::string (palabras).
 *   Operaciones: insertarInicio, insertarFinal, eliminar, buscar, mostrar.
 *   Regla de los tres: copia profunda y destructor que libera cada nodo.
 */
#include <iostream>
#include <string>
using namespace std;

template <typename T>
class ListaEnlazada {
    struct Nodo {
        T dato;
        Nodo* sig;
        Nodo(const T& d, Nodo* s = nullptr) : dato(d), sig(s) {}
    };
    Nodo* cabeza;
    Nodo* cola;
    int tam;

public:
    ListaEnlazada() : cabeza(nullptr), cola(nullptr), tam(0) {}

    ListaEnlazada(const ListaEnlazada& o) : cabeza(nullptr), cola(nullptr), tam(0) {
        for (Nodo* p = o.cabeza; p; p = p->sig) insertarFinal(p->dato);
    }
    ListaEnlazada& operator=(const ListaEnlazada& o) {
        if (this != &o) {
            vaciar();
            for (Nodo* p = o.cabeza; p; p = p->sig) insertarFinal(p->dato);
        }
        return *this;
    }
    ~ListaEnlazada() { vaciar(); }

    void insertarInicio(const T& v) {
        cabeza = new Nodo(v, cabeza);
        if (!cola) cola = cabeza;
        ++tam;
    }

    void insertarFinal(const T& v) {
        Nodo* n = new Nodo(v);
        if (cola) cola->sig = n; else cabeza = n;
        cola = n;
        ++tam;
    }

    // Elimina la primera aparicion de v. Devuelve true si la encontro.
    bool eliminar(const T& v) {
        Nodo* ant = nullptr;
        for (Nodo* p = cabeza; p; ant = p, p = p->sig) {
            if (p->dato == v) {
                if (ant) ant->sig = p->sig; else cabeza = p->sig;
                if (p == cola) cola = ant;
                delete p;
                --tam;
                return true;
            }
        }
        return false;
    }

    bool buscar(const T& v) const {
        for (Nodo* p = cabeza; p; p = p->sig)
            if (p->dato == v) return true;
        return false;
    }

    int tamanio() const { return tam; }

    void mostrar() const {
        cout << "[";
        for (Nodo* p = cabeza; p; p = p->sig)
            cout << p->dato << (p->sig ? " -> " : "");
        cout << "]  (tamanio = " << tam << ")" << endl;
    }

    void vaciar() {
        while (cabeza) {
            Nodo* t = cabeza;
            cabeza = cabeza->sig;
            delete t;
        }
        cola = nullptr;
        tam = 0;
    }
};

int main() {
    cout << "=== Lista de EDADES (ListaEnlazada<int>) ===" << endl;
    ListaEnlazada<int> edades;
    edades.insertarFinal(21);
    edades.insertarFinal(34);
    edades.insertarFinal(19);
    edades.insertarInicio(8);
    edades.mostrar();
    cout << "buscar(34) = " << (edades.buscar(34) ? "si" : "no") << endl;
    cout << "eliminar(19) = " << (edades.eliminar(19) ? "ok" : "no existe") << endl;
    cout << "eliminar(99) = " << (edades.eliminar(99) ? "ok" : "no existe") << endl;
    edades.mostrar();

    cout << "\n=== Lista de PALABRAS (ListaEnlazada<string>) ===" << endl;
    ListaEnlazada<string> palabras;
    palabras.insertarFinal("plantilla");
    palabras.insertarFinal("clase");
    palabras.insertarFinal("objeto");
    palabras.insertarInicio("C++");
    palabras.mostrar();
    cout << "buscar(\"clase\") = " << (palabras.buscar("clase") ? "si" : "no") << endl;
    palabras.eliminar("C++");
    palabras.eliminar("objeto");
    palabras.mostrar();

    cout << "\n=== Copia profunda ===" << endl;
    ListaEnlazada<string> copia = palabras;
    copia.insertarFinal("copiada");
    cout << "original: "; palabras.mostrar();
    cout << "copia   : "; copia.mostrar();
    return 0;
}
