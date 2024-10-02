#include <thread>
#include <iostream>
#include <unistd.h>

#define CANT_THREADS 10

using namespace std;

void thread_f(){
    this_thread::sleep_for(500ms);
    cout<<"Hola soy unthread"<<endl;
}

int main() {
    //Creamos los CANT_THREADS threads
    for (size_t i = 0; i < CANT_THREADS; i++)
    {
        thread* t = new thread(thread_f);

    }
    sleep(1);
    
    return 0;
}