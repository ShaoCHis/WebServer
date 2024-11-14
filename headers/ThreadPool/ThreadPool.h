#ifdef _THREAD_POOL
#define _THREAD_POOL
#include <future>
#include <functional>
#include <iostream>
#include <queue>
#include <mutex>
#include <memory>
#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
using namespace std;

struct task{
    void *next;
    handler_ptr *func;
    void* arg;
}task;

void* task_create();

void* task_destory();

class task_queue{
protected:
    void* _head;
    void** _tail;
    std::mutex _mutex;
}

class ThreadPool{
public:
    ThreadPool(){

    }

    ~ThreadPool(){

    }
protected:
    std::mutex _mutex;
    std::condition_variable _cond;
};


#endif // ThreadPool