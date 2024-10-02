#include <thread>
#include <iostream>
#include <unistd.h>
#include <vector>

#define CANT_THREADS 10

using namespace std;

void thread_f(int i){
    this_thread::sleep_for(1000ms*i);
    cout<<"Hola soy el thread:"<<i<<endl;
}

int main() {
    //Inicio vector de threads y luego los creamos

    vector<thread> threads;
    for (size_t i = 0; i < CANT_THREADS; i++)
    {
        threads.emplace_back(thread_f,i);
        //Version detach()
        //threads[i].detach();    // El thread pasa a ser independiente en recursos al padre.
    }

    //Esperamos a c/u para el join
    for (size_t i = 0; i < CANT_THREADS; i++)
    {
        threads[i].join();
    }
    
    
    return 0;
}