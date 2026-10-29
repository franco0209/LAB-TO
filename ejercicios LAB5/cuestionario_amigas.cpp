#include <iostream>
using namespace std;

template <typename T> class Caja;
template <typename T> void mostrarEnlazada(const Caja<T>& c);   // declaracion previa

template <typename T>
class Caja {
    T valor;
public:
    explicit Caja(T v) : valor(v) {}

    // 1) amiga NO plantilla: una funcion distinta por cada T
    friend void imprimir(const Caja<T>& c) { cout << "imprimir: " << c.valor << endl; }

    // 2) amiga plantilla: TODAS las especializaciones de comparar son amigas
    template <typename U, typename V>
    friend bool igualValor(const Caja<U>& a, const Caja<V>& b);

    // 3) amiga enlazada: solo mostrarEnlazada<T> con el mismo T
    friend void mostrarEnlazada<>(const Caja<T>& c);

    friend ostream& operator<<(ostream& os, const Caja& c) { return os << "Caja(" << c.valor << ")"; }
};

template <typename U, typename V>
bool igualValor(const Caja<U>& a, const Caja<V>& b) { return a.valor == b.valor; }

template <typename T>
void mostrarEnlazada(const Caja<T>& c) { cout << "enlazada: " << c.valor << endl; }

int main() {
    Caja<int> a(7);
    Caja<double> b(7.0);
    imprimir(a);
    mostrarEnlazada(a);
    cout << a << " " << b << endl;
    cout << "igualValor(a, b) = " << boolalpha << igualValor(a, b) << endl;
}
