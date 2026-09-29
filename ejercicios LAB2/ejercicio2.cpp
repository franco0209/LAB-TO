/**
 * ============================================================================
 * ASIGNATURA: TECNOLOGÍA DE OBJETOS
 * PRÁCTICA 02: Puntero en C++
 * ESTUDIANTE: Franco Jesus Cahua Soto
 * ARCHIVO: ejercicio2.cpp
 * DESCRIPCIÓN: Implementación completa de una Lista Doblemente Enlazada
 *              utilizando punteros en C++ con manejo dinámico de memoria:
 *              - Estructura NodoDoble (dato, ant, sig)
 *              - Clase ListaDoblementeEnlazada:
 *                * insertarInicio, insertarFinal, insertarEnOrden
 *                * eliminar, buscar, imprimirAdelante, imprimirAtras
 *                * destructor con liberación de memoria con delete
 * ============================================================================
 */

#include <iostream>

struct NodoDoble {
    int dato;
    NodoDoble* ant;
    NodoDoble* sig;

    NodoDoble(int val) : dato(val), ant(nullptr), sig(nullptr) {}
};

class ListaDoblementeEnlazada {
private:
    NodoDoble* cabeza;
    NodoDoble* cola;
    int cantidad;

public:
    ListaDoblementeEnlazada() : cabeza(nullptr), cola(nullptr), cantidad(0) {}

    ~ListaDoblementeEnlazada() {
        liberarMemoria();
    }

    bool estaVacia() const {
        return cabeza == nullptr;
    }

    int tamano() const {
        return cantidad;
    }

    void insertarInicio(int valor) {
        NodoDoble* nuevo = new NodoDoble(valor);
        if (estaVacia()) {
            cabeza = cola = nuevo;
        } else {
            nuevo->sig = cabeza;
            cabeza->ant = nuevo;
            cabeza = nuevo;
        }
        ++cantidad;
        std::cout << "[INSERTAR INICIO] Valor: " << valor << " insertado exitosamente.\n";
    }

    void insertarFinal(int valor) {
        NodoDoble* nuevo = new NodoDoble(valor);
        if (estaVacia()) {
            cabeza = cola = nuevo;
        } else {
            cola->sig = nuevo;
            nuevo->ant = cola;
            cola = nuevo;
        }
        ++cantidad;
        std::cout << "[INSERTAR FINAL] Valor: " << valor << " insertado exitosamente.\n";
    }

    void insertarEnOrden(int valor) {
        if (estaVacia() || valor <= cabeza->dato) {
            insertarInicio(valor);
            return;
        }
        if (valor >= cola->dato) {
            insertarFinal(valor);
            return;
        }

        NodoDoble* actual = cabeza;
        while (actual != nullptr && actual->dato < valor) {
            actual = actual->sig;
        }

        NodoDoble* nuevo = new NodoDoble(valor);
        nuevo->sig = actual;
        nuevo->ant = actual->ant;
        actual->ant->sig = nuevo;
        actual->ant = nuevo;
        ++cantidad;
        std::cout << "[INSERTAR EN ORDEN] Valor: " << valor << " insertado en posicion ordenada.\n";
    }

    bool eliminar(int valor) {
        if (estaVacia()) {
            std::cout << "[ELIMINAR] Error: La lista se encuentra vacia.\n";
            return false;
        }

        NodoDoble* actual = cabeza;
        while (actual != nullptr && actual->dato != valor) {
            actual = actual->sig;
        }

        if (actual == nullptr) {
            std::cout << "[ELIMINAR] Valor " << valor << " no encontrado en la lista.\n";
            return false;
        }

        if (actual == cabeza && actual == cola) {
            cabeza = cola = nullptr;
        } else if (actual == cabeza) {
            cabeza = cabeza->sig;
            cabeza->ant = nullptr;
        } else if (actual == cola) {
            cola = cola->ant;
            cola->sig = nullptr;
        } else {
            actual->ant->sig = actual->sig;
            actual->sig->ant = actual->ant;
        }

        delete actual;
        --cantidad;
        std::cout << "[ELIMINAR] Valor " << valor << " eliminado correctamente de la lista.\n";
        return true;
    }

    bool buscar(int valor) const {
        NodoDoble* actual = cabeza;
        int posicion = 0;
        while (actual != nullptr) {
            if (actual->dato == valor) {
                std::cout << "[BUSQUEDA] Valor " << valor << " encontrado en la posicion (indice) " << posicion << ".\n";
                return true;
            }
            actual = actual->sig;
            ++posicion;
        }
        std::cout << "[BUSQUEDA] Valor " << valor << " no existe en la lista.\n";
        return false;
    }

    void imprimirAdelante() const {
        std::cout << "\n>>> LISTA HACIA ADELANTE (Cabeza -> Cola) [Total: " << cantidad << "]:\n";
        if (estaVacia()) {
            std::cout << "    [ Lista Vacia ]\n";
            return;
        }
        NodoDoble* actual = cabeza;
        std::cout << "    NULL <-> ";
        while (actual != nullptr) {
            std::cout << "[" << actual->dato << "] <-> ";
            actual = actual->sig;
        }
        std::cout << "NULL\n";
    }

    void imprimirAtras() const {
        std::cout << "\n>>> LISTA HACIA ATRAS (Cola -> Cabeza) [Total: " << cantidad << "]:\n";
        if (estaVacia()) {
            std::cout << "    [ Lista Vacia ]\n";
            return;
        }
        NodoDoble* actual = cola;
        std::cout << "    NULL <-> ";
        while (actual != nullptr) {
            std::cout << "[" << actual->dato << "] <-> ";
            actual = actual->ant;
        }
        std::cout << "NULL\n";
    }

    void liberarMemoria() {
        NodoDoble* actual = cabeza;
        while (actual != nullptr) {
            NodoDoble* siguiente = actual->sig;
            delete actual;
            actual = siguiente;
        }
        cabeza = cola = nullptr;
        cantidad = 0;
    }
};

int main() {
    std::cout << "=========================================================\n";
    std::cout << "  TECNOLOGÍA DE OBJETOS - LAB 02 | EJERCICIO 02\n";
    std::cout << "  Lista Doblemente Enlazada con Punteros\n";
    std::cout << "  Estudiante: Franco Jesus Cahua Soto\n";
    std::cout << "=========================================================\n\n";

    ListaDoblementeEnlazada lista;

    std::cout << "1. Insertando elementos al inicio y al final:\n";
    lista.insertarInicio(20);
    lista.insertarInicio(10);
    lista.insertarFinal(40);
    lista.insertarFinal(50);
    lista.imprimirAdelante();
    lista.imprimirAtras();

    std::cout << "\n2. Insertando elementos en orden (30 y 5):\n";
    lista.insertarEnOrden(30);
    lista.insertarEnOrden(5);
    lista.imprimirAdelante();

    std::cout << "\n3. Buscando elementos en la lista:\n";
    lista.buscar(30);
    lista.buscar(99);

    std::cout << "\n4. Eliminando nodos (inicio: 5, medio: 30, final: 50):\n";
    lista.eliminar(5);
    lista.eliminar(30);
    lista.eliminar(50);
    lista.imprimirAdelante();
    lista.imprimirAtras();

    std::cout << "\n5. Destruccion segura y liberacion de memoria Heap completada.\n";
    return 0;
}
