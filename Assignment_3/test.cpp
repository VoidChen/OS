/*
 * Operating Systems Programming Assignment 3
 * Test
 */
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include "threadpool.h"
using namespace std;

mutex star_mutex;

// Star
class Star: public Job{
    private:
        int size;

    public:
        Star(int size){
            this->size = size;
        }

        void run(){
            lock_guard<mutex> lock(star_mutex);
            for(int i = 0; i < size; ++i)
                cout << '*';
            cout << endl;
        }
};

int main(){
    srand(time(NULL));
    int count = 15;

    //Create thread pool
    Pool *pool = new Pool(4);

    //Submit jobs
    for(int i = 0; i < count; ++i){
        pool->submit(new Star(i+1));
        //usleep((rand()%500)*1000);
    }

    //Wait jobs complete
    for(int i = 0; i < count; ++i)
        pool->job_join(i);

    return 0;
}
