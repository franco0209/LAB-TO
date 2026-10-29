/**
 * ASIGNATURA: TECNOLOGIA DE OBJETOS
 * PRACTICA 05: C++ Templates, Qt / Widgets parte II
 * ESTUDIANTE: Franco Jesus Cahua Soto
 * ARCHIVO: docente_plantillas.cpp
 * DESCRIPCION: Ejemplos resueltos por el docente (guia, seccion II.1):
 *   a) funcion plantilla de aritmetica,
 *   b) funcion plantilla "mayor de",
 *   c) clase plantilla Contenedor<T>.
 * NOTA: en la guia, Contenedor<T>::mostrar() itera con i <= 4 sobre un
 *       arreglo de 4 elementos (indices 0..3): lee fuera de limites (UB).
 *       Aqui se corrige a i < 4 y se inicializa el arreglo.
 */
#include <iostream>
using namespace std;

// ---- a) aritmetica generica ------------------------------------------------
template <typename mitipo>
mitipo aritmetica(mitipo x, mitipo y) {
    return x + y;
}

// ---- b) mayor de dos valores ------------------------------------------------
char mayorLetras(char a, char b) { return (a > b ? a : b); }
int mayorNumeros(int a, int b) { return (a > b ? a : b); }

template <typename Tipo>
Tipo mayorDe(Tipo a, Tipo b) {
    return (a > b ? a : b);
}

// ---- c) clase plantilla -------------------------------------------------------
template <typename T>
class Contenedor {
public:
    Contenedor() : array{} {}               // {} inicializa a cero
    T metodo1(int xx) { return array[xx]; }
    void metodo2(T val, int i) { array[i] = val; }
    void mostrar() const;
private:
    T array[4];
};

template <typename T>
void Contenedor<T>::mostrar() const {
    for (int i = 0; i < 4; i++) {
        cout << array[i] << " ";
    }
    cout << endl;
}

int main() {
    cout << "== a) aritmetica<T> ==" << endl;
    cout << "aritmetica<int>(10, 45)          = " << aritmetica<int>(10, 45) << endl;
    cout << "aritmetica<double>(3.45, 8.10)   = " << aritmetica<double>(3.45, 8.10) << endl;
    cout << "aritmetica<float>(0.14f, 0.147f) = " << aritmetica<float>(0.14f, 0.147f) << endl;

    cout << "\n== b) mayorDe<T> ==" << endl;
    char x = mayorDe<char>('e', 'z');
    int y = mayorDe<int>(9, 4);
    cout << "caracter mayor: " << x << endl;
    cout << "numero mayor: " << y << endl;
    cout << "mayorLetras('e','z') = " << mayorLetras('e', 'z')
         << " | mayorNumeros(9,4) = " << mayorNumeros(9, 4) << endl;
    cout << "mayorDe<double>(2.5, 2.25) = " << mayorDe<double>(2.5, 2.25) << endl;

    cout << "\n== c) Contenedor<T> ==" << endl;
    Contenedor<int> z;
    z.metodo2(10, 0);
    z.metodo2(50, 1);
    z.metodo2(60, 2);
    z.metodo2(70, 3);
    z.mostrar();
    cout << "metodo1(2) = " << z.metodo1(2) << endl;

    Contenedor<double> w;
    w.metodo2(1.5, 0);
    w.metodo2(2.5, 3);
    w.mostrar();
    return 0;
}
