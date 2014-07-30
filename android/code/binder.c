
Binder 用于进程间通信

Binder 框架：

1. Binder 服务端接口
      一个 Binder 服务端实际上就是一个 Binder 类的对象,该对象一旦创建,内部就
    启动一个隐藏线程。该线程接下来会接收 Binder 驱动发送的消息,收到消息后,会执行到 Binder 对象
    中的 onTransact()函数,并按照该函数的参数执行不同的服务代码。因此,要实现一个 Binder 服务,就
    必须重载 onTransact()方法。
      可以想象,重载 onTransact()函数的主要内容是把 onTransact()函数的参数转换为服务函数的参数,
    而 onTransact()函数的参数来源是客户端调用 transact()函数时输入的,因此,如果 transact()有固定格式
    的输入,那么 onTransact()就会有固定格式的输出。

2. Binder 驱动
      任意一个服务端 Binder 对象被创建时,同时会在 Binder 驱动中创建一个
    mRemote 对象,该对象的类型也是 Binder 类。客户端要访问远程服务时,都是通过 mRemote 对象。

3. 应用程序客户端接口
      客户端要想访问远程服务,必须获取远程服务在 Binder 对象中对应的
    mRemote 引用,至于如何获取,下面几节将要介绍。获得该 mRemote 对象后,就可以调用其 transact()
    方法,而在 Binder 驱动中,mRemote 对象也重载了 transact()方法,重载的内容主要包括以下几项。
      1.以线程间消息通信的模式,向服务端发送客户端传递过来的参数。
      2.挂起当前线程,当前线程正是客户端线程,并等待服务端线程执行完指定服务函数后通知(notify)。
      3.接收到服务端线程的通知,然后继续执行客户端线程,并返回到客户端代码区。
