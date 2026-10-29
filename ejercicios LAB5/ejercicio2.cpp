/**
 * ASIGNATURA: TECNOLOGIA DE OBJETOS
 * PRACTICA 05: C++ Templates, Qt / Widgets parte II
 * ESTUDIANTE: Franco Jesus Cahua Soto
 * ARCHIVO: ejercicio2.cpp
 * DESCRIPCION: Transcripcion literal del codigo de la guia (ejercicio 2):
 *   plantilla de clase Contendor<T> y su ESPECIALIZACION TOTAL para char.
 */
#include <iostream>
using namespace std;

template <class T>
class Contendor {
    T elemento;
public:
    Contendor(T arg) { elemento = arg; }
    T add() { return ++elemento; }
};

template <>
class Contendor<char> {
    char elemento;
public:
    Contendor(char arg) { elemento = arg; }
    char uppercase()
    {
        if ((elemento >= 'a') && (elemento <= 'z')) {
            elemento += 'A' - 'a';
        }
        return elemento;
    }
};

int main() {
    Contendor<int> cint(5);
    Contendor<char> cchar('t');
    std::cout << cint.add() << std::endl;
    std::cout << cchar.uppercase() << std::endl;
    // cchar.add();      // ERROR de compilacion: la especializacion NO hereda
    //                   // los miembros de la plantilla generica.
    // cint.uppercase(); // ERROR: Contendor<int> no tiene uppercase().
    return 0;
}
