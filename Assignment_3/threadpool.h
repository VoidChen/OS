/*
 * Operating Systems Programming Assignment 3
 * Thread Pool
 */
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

class Job{
    friend class Pool;

    private:
        string status;

    public:
        int id;

        virtual void run(){
        }

        Job(){
            id = 0;
            status = "READY";
        }
};

class Pool;
void loop_run(Pool *pool);

class Pool{
    private:
        int thread_pool_size;
        thread *thread_list;

        vector<Job*> job_list;
        unsigned int job_ready_flag;
        mutex job_list_mutex;

        condition_variable wait_job;
        condition_variable wait_complete;

        //Create threads
        void create_thread(){
            thread_list = new thread[thread_pool_size];
            for(int i = 0; i < thread_pool_size; ++i)
                thread_list[i] = thread(loop_run, this);
        }

    public:
        //Initial thread pool
        Pool(int size = 4){
            thread_pool_size = size;
            job_ready_flag = 0;
            create_thread();
        }

        //Submit new job
        int submit(Job *job){
            lock_guard<mutex> job_list_guard(job_list_mutex);
            job_list.push_back(job);
            job_list.back()->id = job_list.size()-1;
            wait_job.notify_all();
            return job_list.back()->id;
        }

        //Wait job complete
        void job_join(int job_id){
            unique_lock<mutex> lock(job_list_mutex);
            while(job_list[job_id]->status != "COMPLETE")
                wait_complete.wait(lock);
        }

        //Thread get job
        Job* get_job(){
            unique_lock<mutex> lock(job_list_mutex);
            while(job_ready_flag == job_list.size())
                wait_job.wait(lock);
            job_list[job_ready_flag]->status = "RUNNING";
            return job_list[job_ready_flag++];
        }

        //Thread complete job
        void set_complete(int job_id){
            lock_guard<mutex> job_list_guard(job_list_mutex);
            job_list[job_id]->status = "COMPLETE";
            wait_complete.notify_all();
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
