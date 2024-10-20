#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cstddef>
#include <mutex>

using namespace std;

struct Auto {
    string id;
    int distancia_recorrida;
    int lugar_llegada;
};

void carrera(Auto &auto_info, int distancia_total, int &posicion_llegada, mutex &mtx) {
    while (auto_info.distancia_recorrida < distancia_total) {
        int avance = rand() % 10 + 1;  // Avanza entre 1 y 10 metros
        auto_info.distancia_recorrida += avance;
        int espera = rand() % 400 + 100;  // Tiempo de espera entre 100 y 500 ms
        this_thread::sleep_for(chrono::milliseconds(espera));

        // Usamos un mutex para bloquear la impresión y evitar conflictos entre hebras
        mtx.lock();
        cout << auto_info.id << " avanza " << avance << " metros (total: " << auto_info.distancia_recorrida << " metros)" << endl;
        if (auto_info.distancia_recorrida >= distancia_total) {
            auto_info.lugar_llegada = ++posicion_llegada;
            cout << auto_info.id << " termina la carrera en el lugar " << auto_info.lugar_llegada << "!" << endl;
        }
        mtx.unlock();
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Uso: " << argv[0] << " <distancia total> <numero de autos>" << endl;
        return 1;
    }

    int distancia_total = stoi(argv[1]);
    int numero_autos = stoi(argv[2]);
    srand(time(0));

    vector<Auto> autos(numero_autos);
    for (int i = 0; i < numero_autos; ++i) {
        autos[i] = { "Auto" + to_string(i), 0, 0 };
    }

    int posicion_llegada = 0;
    vector<thread> hebras;
    mutex mtx;  // Mutex para la sincronización de salida

    // Creamos una hebra para cada auto
    for (int i = 0; i < numero_autos; ++i) {
        hebras.push_back(thread(carrera, ref(autos[i]), distancia_total, ref(posicion_llegada), ref(mtx)));
    }

    // Esperamos que todas las hebras terminen
    for (auto &t : hebras) {
        t.join();
    }

    // Mostramos los resultados finales de la carrera
    cout << "Resultados finales de la carrera:" << endl;
    cout << "Lugar | Auto" << endl;
    for (const auto &auto_info : autos) {
        cout << auto_info.lugar_llegada << " | " << auto_info.id << endl;
    }

    return 0;
}
