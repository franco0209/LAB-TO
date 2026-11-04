// Ejercicio 1: lista simple enlazada con 4 hilos concurrentes (std::mutex)
//   Hilo 1: elimina | Hilo 2: inserta | Hilo 3: busca | Hilo 4: modifica
#include <iostream>
#include <thread>
#include <mutex>
#include <random>
#include <vector>
#include <functional>
#include <chrono>

class ListaSegura {
    struct Nodo {
        int dato;
        Nodo* sig;
        explicit Nodo(int d) : dato(d), sig(nullptr) {}
    };
    Nodo* cabeza = nullptr;
    int tam = 0;
    mutable std::mutex mtx;                 // protege la lista y la salida
public:
    ListaSegura() = default;
    ListaSegura(const ListaSegura&) = delete;
    ListaSegura& operator=(const ListaSegura&) = delete;
    ~ListaSegura() {
        while (cabeza) { Nodo* t = cabeza; cabeza = cabeza->sig; delete t; }
    }

    void insertar(int v, const char* hilo = "Hilo 2") {
        std::lock_guard<std::mutex> lk(mtx);
        Nodo* n = new Nodo(v);
        n->sig = cabeza;
        cabeza = n;
        ++tam;
        std::cout << "[" << hilo << "] Insertando: " << v << "\n";
    }
    bool eliminar(int v) {
        std::lock_guard<std::mutex> lk(mtx);
        Nodo* ant = nullptr;
        for (Nodo* p = cabeza; p; ant = p, p = p->sig) {
            if (p->dato == v) {
                if (ant) ant->sig = p->sig; else cabeza = p->sig;
                delete p; --tam;
                std::cout << "[Hilo 1] Eliminando: " << v << "\n";
                return true;
            }
        }
        std::cout << "[Hilo 1] No se elimino " << v << "\n";
        return false;
    }
    bool buscar(int v) const {
        std::lock_guard<std::mutex> lk(mtx);
        for (Nodo* p = cabeza; p; p = p->sig)
            if (p->dato == v) { std::cout << "[Hilo 3] Buscado: " << v << "\n"; return true; }
        std::cout << "[Hilo 3] No encontrado: " << v << "\n";
        return false;
    }
    bool modificar(int v, int suma) {
        std::lock_guard<std::mutex> lk(mtx);
        for (Nodo* p = cabeza; p; p = p->sig)
            if (p->dato == v) {
                p->dato += suma;
                std::cout << "[Hilo 4] Modificando " << v << " a " << p->dato << "\n";
                return true;
            }
        std::cout << "[Hilo 4] No se encontro " << v << "\n";
        return false;
    }
    void mostrar(const char* titulo) const {
        std::lock_guard<std::mutex> lk(mtx);
        std::cout << titulo << " (tam=" << tam << "): [";
        for (Nodo* p = cabeza; p; p = p->sig) std::cout << p->dato << (p->sig ? ", " : "");
        std::cout << "]\n";
    }
};

constexpr int OPERACIONES = 10;
constexpr int MAXV = 20;                    // rango de valores aleatorios

// pausa breve fuera de la seccion critica para que los hilos se intercalen
void pausa() { std::this_thread::sleep_for(std::chrono::milliseconds(2)); }

void tareaEliminar(ListaSegura& l) {
    std::mt19937 g(101); std::uniform_int_distribution<int> d(1, MAXV);
    for (int i = 0; i < OPERACIONES; ++i) { l.eliminar(d(g)); pausa(); }
}
void tareaInsertar(ListaSegura& l) {
    std::mt19937 g(202); std::uniform_int_distribution<int> d(1, MAXV);
    for (int i = 0; i < OPERACIONES; ++i) { l.insertar(d(g)); pausa(); }
}
void tareaBuscar(ListaSegura& l) {
    std::mt19937 g(303); std::uniform_int_distribution<int> d(1, MAXV);
    for (int i = 0; i < OPERACIONES; ++i) { l.buscar(d(g)); pausa(); }
}
void tareaModificar(ListaSegura& l) {
    std::mt19937 g(404); std::uniform_int_distribution<int> d(1, MAXV);
    for (int i = 0; i < OPERACIONES; ++i) { l.modificar(d(g), 100); pausa(); }
}

int main() {
    ListaSegura lista;
    for (int v : {3, 7, 12, 15, 18}) lista.insertar(v, "Inicial");   // datos iniciales
    lista.mostrar("Lista inicial");
    std::cout << "---- 4 hilos en paralelo ----\n";

    std::vector<std::thread> hilos;
    hilos.emplace_back(tareaEliminar,  std::ref(lista));
    hilos.emplace_back(tareaInsertar,  std::ref(lista));
    hilos.emplace_back(tareaBuscar,    std::ref(lista));
    hilos.emplace_back(tareaModificar, std::ref(lista));
    for (auto& h : hilos) h.join();

    std::cout << "---- fin ----\n";
    lista.mostrar("Lista final");
    return 0;
}
