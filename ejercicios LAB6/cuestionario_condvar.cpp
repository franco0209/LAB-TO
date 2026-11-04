// Pregunta 2: productor/consumidor con std::condition_variable
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

std::mutex mtx;
std::condition_variable cv;
std::queue<int> cola;
bool terminado = false;

void productor() {
    for (int i = 1; i <= 5; ++i) {
        {
            std::lock_guard<std::mutex> lk(mtx);
            cola.push(i * 10);
            std::cout << "Productor genera " << i * 10 << "\n";
        }
        cv.notify_one();                              // despierta al consumidor
    }
    { std::lock_guard<std::mutex> lk(mtx); terminado = true; }
    cv.notify_one();
}

void consumidor() {
    for (;;) {
        std::unique_lock<std::mutex> lk(mtx);
        cv.wait(lk, [] { return !cola.empty() || terminado; });  // espera sin consumir CPU
        while (!cola.empty()) {
            std::cout << "Consumidor recibe " << cola.front() << "\n";
            cola.pop();
        }
        if (terminado) break;
    }
}

int main() {
    std::thread c(consumidor), p(productor);
    p.join(); c.join();
    std::cout << "Fin\n";
    return 0;
}
