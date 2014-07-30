一、Socket通信简介 

  Android与服务器的通信方式主要有两种，一是Http通信，一是Socket通信。
两者的最大差异在于，http连接使用的是“请求—响应方式”，即在请求时建立连接通道，
当客户端向服务器发送请求后，服务器端才能向客户端返回数据。而Socket通信则是在
双方建立起连接后就可以直接进行数据的传输，在连接时可实现信息的主动推送，而不
需要每次由客户端向服务器发送请求。

什么是Socket
  是一种抽象层，应用程序通过它来发送和接收数据，使用Socket可以将应用程序添加
到网络中，与处于同一网络中的其他应用程序进行通信。简单来说，Socket提供了程序
内部与外界通信的端口并为通信双方的提供了数据传输通道。

3.1 基于TCP协议的Socket
  服务器端首先声明一个ServerSocket对象并且指定端口号，然后调用Serversocket的
accept()方法接收客户端的数据。accept()方法在没有数据进行接收的处于堵塞状态。

一旦接收到数据，通过 inputstream 读取接收的数据。

客户端创建一个Socket对象，指定服务器端的ip地址和端口号，通过inputstream 读取数据，
（Socket socket=new Socket("172.168.10.108",8080)）
，获取服务器发出的数据

eg:
服务器端
    protected ServerSocket mServer;
    mServer = new ServerSocket(mPort, int backlog); //mport 为int，端口

    xxx Socket client = mServer.accept();


3.2 基于UDP协议的数据传输
  服务器端首先创建一个DatagramSocket对象，并且指点监听的端口。接下来创建一个空的
DatagramSocket对象用于接收数据
