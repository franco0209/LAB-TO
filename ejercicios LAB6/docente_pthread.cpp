// Ejemplo a) del docente: procesos paralelos con POSIX <pthread.h>
// (version finita: el original usa un bucle infinito)
#include <pthread.h>
#include <time.h>
#include <iostream>
#include <cstdio>

void* procesoHilo(void* arg) {
    const char* nombre = static_cast<const char*>(arg);
    timespec espera;
    espera.tv_sec = 0;
    espera.tv_nsec = 200000000;               // 200 ms
    for (int i = 1; i <= 3; ++i) {
        std::printf("%s: iteracion %d\n", nombre, i);
        std::fflush(stdout);
        nanosleep(&espera, nullptr);
    }
    return nullptr;
}

int main() {
    pthread_t proceso1, proceso2;
    pthread_create(&proceso1, nullptr, procesoHilo, (void*)"proceso1");
    pthread_create(&proceso2, nullptr, procesoHilo, (void*)"proceso2");
    pthread_join(proceso1, nullptr);
    pthread_join(proceso2, nullptr);
    std::cout << "Ambos procesos terminaron\n";
    return 0;
}
