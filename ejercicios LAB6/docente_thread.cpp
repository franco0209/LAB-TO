// Ejemplo b) del docente: procesos paralelos con <thread>
#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtxSalida;

void ExecuteThread(int id) {
    std::lock_guard<std::mutex> lock(mtxSalida);   // evita mezclar la salida
    std::cout << "Ejecutando hilo con id = " << id << std::endl;
}

int main() {
    std::thread th1(ExecuteThread, 1);
    std::thread th2(ExecuteThread, 2);
    th1.join();
    th2.join();
    std::cout << "Fin del programa principal\n";
    return 0;
}
