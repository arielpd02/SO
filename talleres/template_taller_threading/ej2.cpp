#include <vector>
#include <thread>
#include <iostream>
#include <semaphore.h>

#define MSG_COUNT 5
#define GLOBAL 1
using namespace std;

sem_t esperarA;
sem_t esperarB;



void f1_a(){
    for (size_t i = 0; i < MSG_COUNT; i++)
    {
        cout<<"Ejecutando F1 (A)\n";
        this_thread::sleep_for(100ms);
    }
}

void f1_b(){
    for (size_t i = 0; i < MSG_COUNT; i++)
    {
        cout<<"Ejecutando F1 (B)\n";
        this_thread::sleep_for(200ms);
    }
}

void f2_a(){
    for (size_t i = 0; i < MSG_COUNT; i++)
    {
        cout<<"Ejecutando F2 (A)\n";
        this_thread::sleep_for(500ms);
    }
}

void f2_b(){
    for (size_t i = 0; i < MSG_COUNT; i++)
    {
        cout<<"Ejecutando F2 (B)\n";
        this_thread::sleep_for(10ms);
    }
}

void f1() {
    f1_a();
    sem_post(&esperarA);
    sem_wait(&esperarB);
    f1_b();
    return;
}

void f2() {
    f2_a();
    sem_wait(&esperarA);
    sem_post(&esperarB);
    f2_b();
    return;
}

int main() {

    sem_init(&esperarB,GLOBAL,0);
    sem_init(&esperarA,GLOBAL,0);
    
    //Creamos dos threads; f1 y f2
    vector<thread> threads;
    threads.emplace_back(f1);
    threads.emplace_back(f2);

    //Esperamos a que joinearlos
    threads[0].join();
    threads[1].join();

    sem_destroy(&esperarA);
    sem_destroy(&esperarB);

    return 0;
}