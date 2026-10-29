/**
 * ASIGNATURA: TECNOLOGIA DE OBJETOS
 * PRACTICA 05: C++ Templates, Qt / Widgets parte II
 * ESTUDIANTE: Franco Jesus Cahua Soto
 * ARCHIVO: ejercicio3.cpp
 * DESCRIPCION: Transcripcion del codigo de la guia (ejercicio 3):
 *   plantilla con parametro de tipo por defecto (T = char) y parametro
 *   NO tipo (int N = 8) que fija el tamanio del arreglo en compilacion.
 * NOTA: en la guia, `obj.get(4)` lee un elemento NUNCA asignado de un
 *       objeto local (valor indeterminado, comportamiento indefinido).
 *       Se conserva la llamada para analizarla, pero se agrega `objCero`
 *       con inicializacion por valor ({}) que garantiza ceros.
 */
#include <iostream>
using namespace std;

template <class T = char, int N = 8>
class Comun1 {
    T bloque[N];
public:
    void set(int num, T tval);
    T get(int num);
};

template <class T, int N>
void Comun1<T, N>::set(int num, T tval) {
    bloque[num] = tval;
}

template <class T, int N>
T Comun1<T, N>::get(int num) {
    return bloque[num];
}

int main() {
    Comun1<int, 5> objInt;
    Comun1<double, 5> objFloat;
    Comun1<> obj;                 // equivale a Comun1<char, 8>
    objInt.set(0, 10);
    objFloat.set(2, 3.1);

    std::cout << objInt.get(0) << std::endl;
    std::cout << objFloat.get(2) << std::endl;
    std::cout << (int)obj.get(4) << "  <- indeterminado (UB); se castea a int para verlo" << std::endl;

    Comun1<> objCero{};           // inicializacion por valor: todo en cero
    std::cout << (int)objCero.get(4) << "  <- con {} siempre es 0" << std::endl;
    std::cout << "sizeof(Comun1<int,5>)    = " << sizeof(objInt) << " bytes" << std::endl;
    std::cout << "sizeof(Comun1<double,5>) = " << sizeof(objFloat) << " bytes" << std::endl;
    std::cout << "sizeof(Comun1<>)         = " << sizeof(obj) << " bytes" << std::endl;
    return 0;
}
