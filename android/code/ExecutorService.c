
如果你有许多需要长时间运行的任务同时执行，并需要等所有的这些线程都执行完毕，
还想得到一个返回值，那就是 Executors ，一个简单的类可以让你创建线程池和线程工厂。

一.ExecutorService: 线程池，提交任务，并进行调度执行

  它也是一个接口，它扩展自Executor接口，Executor接口更像一个抽象的命令模式，
仅有一个方法:execute(runnable);Executor接口简单，但是很重要，重要在这种设计的模式上。

获得对象：
    ExecutorService es = Executors.newFixedThreadPool(int nThreads);
                            //此方法，实际上是创建ThreadPoolExecutor实例(向上转型)
                            //ThreadPoolExecutor类是ExecutorService的子类
    
    Single Thread Executor : 只有一个线程的线程池，因此所有提交的任务是顺序执行
        代码： Executors.newSingleThreadExecutor()
    Cached Thread Pool : 线程池里有很多线程需要同时执行，老的可用线程将被新的
    任务触发重新执行，如果线程超过60秒内没执行，那么将被终止并从池中删除，
        代码：Executors.newCachedThreadPool()
    Fixed Thread Pool : 拥有固定线程数的线程池，如果没有任务执行，那么线程会一直等待，
        代码： Executors.newFixedThreadPool()
    Scheduled Thread Pool : 用来调度即将执行的任务的线程池，
        代码：Executors.newScheduledThreadPool()
    Single Thread Scheduled Pool : 只有一个线程，用来调度执行将来的任务，
        代码：Executors.newSingleThreadScheduledExecutor()


ExecutorService在Executor的基础上增加了“service”特性的方法：
    
  1. Future submit(callable/runnale):  向Executor提交任务，并返回一个结果未定的Future。
  
  shutdown()、shutdownNow():都是关闭当前service服务，释放Executor的所有资源（参见实现类）；
它所触发的动作就是取消队列中任务的执行。shutdown是一种“友好”的关闭，它将不再（事实上是不能）
接受新的任务提交，同时把已经提交到队列中的任务执行完毕。shutdownNow更加直接一些，它将会把尚
未执行的任务不再执行，正在执行的任务，通过“线程中断”（thread.interrupt）,如果线程无法响应“中断”，
那么将不会通过此方式被立即结束。shutdowNow是个有返回类型的方法，它返回那些等待执行的任务列表
（List<Runnable>）
  
  isShutdown():程序是否已经关闭

  isTerminated():是否已经结束，如果关闭后，所有的任务都执行完成，将返回true，否则其他情况均返回false。
  
  awaitTermination(timeout):会抛出interruptException，此方法就是个废柴，
大概意思是等待一段之间直到“任务全部结束”，如果超时就返回false。
  
  List<Future> invokeAll(Collection<Callable>):一个废柴方法，同步的方法，
执行所有的任务列表，当所有任务都执行完成后，返回Future列表。这方法有啥用？？貌似，
可以对一批任务进行批量跟踪。此方法会抛出interruptException。

  T invokeAny(Collection<Callable>): 任务集合中，任何一个任务完成就返回。

二.ThreadPoolExecutor:
    ExecutorService其中最重要的实现类，就是ThreadPoolExecutor（线程池执行器），
我们使用Executor，无非也就是使用线程池Executor。

三.ScheduledExecutorService:
  ExecutorService接口有一个非常重要的子接口： ScheduledExecutorService，
从它的名字，我们就能看出此service是为了支持时间可控的任务执行而设计，其中包括：
固定延迟执行，周期性执行；不过他还不支持制定特定date执行，这个工作可以交给Timer;

  ScheduledExecutorService 提供了如下3个核心方法：

    1. schedule(Callable<V> callable, long delay, TimeUnit unit)
       schedule(Runnable command, long delay, TimeUnit unit)
    2. scheduleAtFixedRate(Runnable command, long initialDelay, long period, TimeUnit unit)
    3. scheduleWithFixedDelay(Runnable command, long initialDelay, long delay, TimeUnit unit)

