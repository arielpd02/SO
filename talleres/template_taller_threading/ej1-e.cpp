#include <thread>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <mutex>

/*OBS -> esta salida no permite necesariamente la concurrencia.La seccion CRIT pasa a ser el thread en si...*/

#define CANT_THREADS 10

using namespace std;

mutex mtx;

void thread_f(int k){
    for (size_t i = 0; i < 5; i++)
    {    
        lock_guard<mutex> lk(mtx);  //Esta clase implicitamente hace el unlock del mtx en el momento que se llama al destructor.
        cout<<"Hola! soy el thread:"<<k<<endl;
        this_thread::sleep_for(100ms);
        cout<<"Chau! saludos desde:"<<k<<endl;
    }    
    return;
}

int main() {
    //Inicio vector de threads y luego los creamos
    vector<thread> threads;

    for (size_t i = 0; i < CANT_THREADS; i++)
    {
        threads.emplace_back(thread_f,i);
    }

    //Esperamos a c/u para el join
    for (size_t i = 0; i < CANT_THREADS; i++)
    {
        threads[i].join();
    }
    
    
    return 0;
}