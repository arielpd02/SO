#include <assert.h>
#include <thread>
#include <iostream>
#include <vector>
#include <numeric>
#include <iterator>
#include <atomic>


#define N 100
#define THREAD_COUNT 5

using namespace std;

atomic<int> ultimaPos=0;
vector<int> verifier(THREAD_COUNT,0);

void verify(vector<int> v1, vector<int> v2, vector<int> v3) {
    cout << "v1 = [" << v1[0] << ", " << v1[1] << ", " << ", ..., " << v1[N - 2] << ", " << v1[N - 1] << "]" << endl;
    cout << "v2 = [" << v2[0] << ", " << v2[1] << ", " << ", ..., " << v2[N - 2] << ", " << v2[N - 1] << "]" << endl;
    cout << "v3 = [" << v3[0] << ", " << v3[1] << ", " << ", ..., " << v3[N - 2] << ", " << v3[N - 1] << "]" << endl;

    cout<<"[";
    for (size_t i = 0; i < THREAD_COUNT; i++)
    {
        cout<<verifier[i]<<',';
    }
    cout<<"]"<<endl;    

    for (int i = 0; i < N; ++i) {
        assert(v3[i] == v1[i] * v2[i]);
    }
}

void multiply(vector<int> &v1,vector<int> &v2,vector<int> &v3,int nro_thread){
    for(int i=ultimaPos.load();i<N;i++){   
        i=ultimaPos.fetch_add(1);     //Sumamos uno a la atomica.
        v3[i]=v1[i]*v2[i];
        verifier[nro_thread]++;
        this_thread::sleep_for(100ms*(i+1));
    }
    return;    
}



int main() {
    vector<int> v1(N);
    vector<int> v2(N);
    iota(v1.begin(), v1.end(), 1);
    iota(v2.begin(), v2.end(), N + 1);

    vector<int> v3(N);

    //Creamos los threads y les pasamos sus boundaries
    vector<thread> threads;
    for (size_t i = 0; i < THREAD_COUNT; i++)
    {
        threads.emplace_back(multiply,ref(v1),ref(v2),ref(v3),i);
    }

    for (size_t i = 0; i < THREAD_COUNT; i++)
    {
        threads[i].join();
    }
    

    verify(v1, v2, v3);

    return 0;
}