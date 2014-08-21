Handler、Looper、MessageQueue 异步消息处理机制

功能：实现线程间通信

Handler把消息对象加入消息队列
Looper不停从消息队列中取出对象，没有对象则等待
每取出一个消息Looper调用交给Handler去处理

重要方法：
    @Override
    public void handleMessage(Message msg);

使用：
  1. 主线程创建一个类 继承 handler类， 复写 handleMeassage(Message msg) 如打印消息等
  2. 主线程创建其对象 handler
  3. 工作线程创建Message对象： 
        Message msg = handler.obtainMessage();
		msg.what = 2;
		handler.sendMessages(msg); //将消息放入消息队列
					     //Looper将会取出msg，找出与之对应的handler
					     //Looper将会调用其handler.handleMessage(Message msg);

eg:
    内部类NetworkThread继承Thread，复写run方法。休眠2秒，s的值模拟从网络中获取的数据。
s的值放到textview中，不能直接在改线程中更新ui的内容。

button监听器的onClickListener方法中，定义NetworkThread线程对象并启动

定义一个Handler的实现类继承Handler,实现handleMessage（）方法。

用MyHandler定义handler对象。

网络线程中定义msg，handler.obtionMessage（）生成，发送消息（handler.sendMessage（）），msg作为参数传入。sendMessage（）方法在主线程或WorkerThread中发送都是可以的。

主线程中接收消息--handler在主线程中定义，handleMessage处理消息也是在主线程。建立起两个线程之间的通道。



相反例子：从mainthread向workerthread发送消息。

1 准备looper对象

2 在WorkerThread当中生成Handler对象

3 在MainThread当中发送消息

在WorkerThread的run方法中执行Looper.prepare（）；就生成一个Looper对象，并完成该对象的初始化工作。

在WorkerThread中生成handler对象。用匿名内部类。handler = new Handler(){
						public void handleMessages(Message msg){ }
						}

Looper调用loop（）方法。之后，looper对象将不断从消息队列中取出消息对象。然后调用handler的handleMessage（）方法处理消息对象（在worker thread中）。如果消息队列中没有对象，则该线程阻塞。

主线程中定义workerthread并启动。

button监听器的onClick()方法，生成msg和发送消息。

Looper.loop();

onClick运行在主线程，handleMessage运行在worker thread，发送、接收消息在不同线程。

在哪个线程中实现handler对象new Handler（）---实例化，并复写handleMessage（）方法，就在哪个线程中处理消息。handler定义在主线程。

从主线程向worker thread发送消息，worker thread中run方法里固定三个步骤：

（1）Looper.prepare；

（2）handler = new MyHandler（）；（或者用匿名内部类new Handler（）{}）

（3）Looper.loop（）；

