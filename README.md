# WebServer
C++ WebServer Project:ThreadPoll,Coroutine,Reactor(Epoll),Timer and maybe MemoryPoll


>知识补充 Reactor与Proactor
>
>Reactor：要求主线程（I/O处理单元）只负责监听文件描述符上是否有事件发生（可读、可写），若有，则立即通知工作线程（逻辑单元），将socket可读可写事件放入请求队列，交给工作线程处理
>
>Proactor：将所有的I/O操作都交给主线程和内核来处理（进行读写），工作线程仅负责处理逻辑，如主线程都完成后`user[socket].read()`，选择一个工作线程来处理客户请求`pool->append(users+socket)`
>
>通常使用同步I/O模型（如epoll_wait）实现Reactor，使用异步I/O（如`aio_read`和`aio_write`）实现Proactor
>
>同步（阻塞）I/O：在一个线程中，CPU执行代码的速度极快，然而，一旦遇到IO操作，如读写文件、发送网络数据时，就需要等待IO操作完成，才能继续进行下一步操作。这种情况称为同步IO。
>
>异步（非阻塞）I/O：当代码需要执行一个耗时的IO操作时，它只发出IO指令，并不等待IO结果，然后就去执行其他代码了。一段时间后，当IO返回结果时，再通知CPU进行处理。
>
>对于`select`和`poll`来说，所有文件描述符都是在**用户态被加入其文件描述符集合的**，每次调用都需要将整个集合**拷贝到内核态**；`epoll`则将**整个文件描述符集合维护在内核态**，每次添加文件描述符的时候都需要执行一个**系统调用**。系统调用的开销是很大的，而且在有很多短期活跃连接的情况下，epoll可能会慢于select和poll由于这些大量的系统调用开销。
>
>`select`使用**线性表**描述文件描述符集合，文件描述符有上限；`poll`使用**链表**来描述；`epoll`底层通过**红黑树**来描述，并且维护一个ready list，将事件表中已经就绪的事件添加到这里，在使用epoll_wait调用时，仅观察这个list中有没有数据即可。
>
>`select`和`poll`的最大开销来自内核判断是否有文件描述符就绪这一过程：每次执行select或poll调用时，它们会采用**遍历**的方式，遍历整个文件描述符集合去判断各个文件描述符是否有活动；`epoll`则不需要去以这种方式检查，当有活动产生时，会自动触发**epoll回调函数**通知epoll文件描述符，然后内核将这些就绪的文件描述符放到之前提到的`ready list`中等待epoll_wait调用后被处理。
>
>`select`和`poll`都只能工作在相对低效的`LT`模式下，而`epoll`同时支持`LT`和`ET`模式。
>
>综上，当监测的fd数量较小，且各个fd都很活跃的情况下，建议使用select和poll；当监听的fd数量较多，且单位时间仅部分fd活跃的情况下，使用epoll会明显提升性能
>
><span style = "color:red">ET比LT高效？ET可以有效避免系统饥饿现象的发生，同时，ET在处理大数据交互时可以大量减少`epoll_wait`无效的苏醒次数；但是效率问题，ET理论上可以比LT减少系统调用，例如epoll事件调用后，线程执行，如果ET模式则会将事件从`ready_list`中删除，但LT模式仍会存在，增加轮询开销？ </span>

>GET和POST的区别
>- 最直观的区别就是GET把参数包含在URL中，POST通过request body传递参数。
>- GET请求参数会被完整保留在浏览器历史记录里，而POST中的参数不会被保留。
>- GET请求在URL中传送的参数是有长度限制。（大多数）浏览器通常都会限制url长度在2K个字节，而（大多数）服务器最多处理64K大小的url。
>- GET产生**一个TCP数据包**；POST产生**两个TCP数据包**。对于GET方式的请求，浏览器会把http header和data一并发送出去，服务器响应200（返回数据）；而对于POST，浏览器先发送header，服务器响应100（指示信息—表示请求已接收，继续处理）continue，浏览器再发送data，服务器响应200 ok（返回数据）。


## 如何完成？
- Mysql 数据库 后端存储 **数据库连接池**
- Coroutine 实现fd的读写请求？
- ThreadPoll 进行数据的请求与返回
- Reactor 进行处理
- Timer 定时器 当某个连接长时间不交换数据，一直占用，就应该利用定时器将对应的连接释放
- 解析HTTP请求 request
- CGI（通用网关接口）

