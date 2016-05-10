/*
 * Operating Systems Programming Assignment 2
 * Thread Pool
 */
#include <iostream>
#include <unistd.h>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

mutex cout_lock;

class Job{
    friend class Job_List;

    private:
        string status;

    public:
        int id;

        virtual void run(){
            lock_guard<mutex> list_guard(cout_lock);
            cout << "Job " << id << endl;
        }

        Job(){
            id = 0;
            status = "READY";
        }
};

class Job_List{
    private:
        vector<Job*> list;
        unsigned int ready_flag;
        mutex list_mutex;
        condition_variable wait_job;
        condition_variable wait_complete;

    public:
        Job_List(){
            ready_flag = 0;
        }

        int append(Job *job){
            lock_guard<mutex> list_guard(list_mutex);
            list.push_back(job);
            list.back()->id = list.size()-1;
            wait_job.notify_all();
            return list.back()->id;
        }

        void join(int job_id){
            unique_lock<mutex> lock(list_mutex);
            while(list[job_id]->status != "COMPLETE")
                wait_complete.wait(lock);
        }

        Job* get_job(){
            unique_lock<mutex> lock(list_mutex);
            while(ready_flag == list.size())
                wait_job.wait(lock);
            list[ready_flag]->status = "RUNNING";
            return list[ready_flag++];
        }

        void set_complete(int job_id){
            lock_guard<mutex> list_guard(list_mutex);
            list[job_id]->status = "COMPLETE";
            wait_complete.notify_all();
        }
};

class Pool;
void loop_run(Pool *pool);

class Pool{
    public:
        int size;
        thread *thread_list;
        Job_List job_list;

        Pool(int size){
            this->size = size;
            thread_list = new thread[size];
            for(int i = 0; i < size; ++i)
                thread_list[i] = thread(loop_run, this);
        }

        int submit(Job *newjob){
            return job_list.append(newjob);
        }

        void job_join(int job_id){
            job_list.join(job_id);
        }

        Job* get_job(){
            return job_list.get_job();
        }

        void set_complete(int job_id){
            job_list.set_complete(job_id);
        }
};

void loop_run(Pool *pool){
    Job *job;
    while(true){
        job = pool->get_job();
        job->run();
        pool->set_complete(job->id);
    }
}

mutex star_mutex;

class Star: public Job{
    public:
        int size;

        void run(){
            lock_guard<mutex> lock(star_mutex);
            for(int i = 0; i < size; ++i)
                cout << '*';
            cout << endl;
        }

        Star(int n){
            size = n;
        }
};

int main(){
    Pool *pool = new Pool(4);
    for(int i = 0; i < 10; ++i)
        pool->submit(new Star(i));
    for(int i = 0; i < 10; ++i)
        pool->job_join(i);
    return 0;
}
