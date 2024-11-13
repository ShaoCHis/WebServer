## 接口问题
>需要考虑对用户暴露的接口问题，接口的创建需要对称

针对资源的创建采用回滚式编程，业务逻辑采用防御式编程

```c++
//有创建
task_create(){

}

//就有销毁
task_destory(){

}

//唤醒阻塞线程，需要在有锁条件下操作
nonblock(*queue){
}

//添加队列
add_task(*queue,*task){
    // 不限定任务类型，只要该任务的结构起始内存是一个用于链接的指针
    void** link = (void**)task;
    *link = NULL;

    spinlock_lock(&queue->lock);
    *queue->tail /* 等价于 queue->tail->next*/ = link;
    queue->tail = link;
    spinlock_unlock(&queue->lock);
}

pop_task();

get_task();

thread_create();

//向线程池内部加入任务
thread_post();


```


队列实现
```c++
void **tail;
//指向指针类型的指针，指针类型占用多少个字节？ 64位，占用8字节
typedef struct task_s{
    void* next;
    handler_pt func;
    void *arg;
}task_t;


task_t* ptail = &ptask;
task_t **tail = &ptask;
/***
 * 1.ptail   tail 起始位置是不是一样的？  ----》起始位置是一样的
 * 2.编译器看来 ptail 的内存范围 tail 的内存范围 谁大？ ----》ptail的内存范围为task_t 的大小，tail的内存范围为 8 字节；
 * TODO: tail 占用8字节，实际指向的地址即为 *next指针
 * /
```


