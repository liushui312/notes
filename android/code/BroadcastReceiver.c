
  在Android中，Broadcast是一种广泛运用的在应用程序之间传输信息的机制，
而BroadcastReceiver是对发送出来的 Broadcast 进行过滤接受并响应的一类组件。

1. 首先在需要发送信息的地方，把要发送的信息和用于过滤的信息(如Action、Category)装入一个Intent对象
然后通过调用 sendBroadcast()或sendStickyBroadcast()方法，把 Intent对象以广播方式发送出去。

    Intent intent = new Intent(actionString);          //指定广播目标Action
    intent.putExtra("msg", "hi,我通过广播发送消息了"); //并且可通过Intent携带消息
    Context.sendBroadcast(intent);                     //发送广播消息

2. 当Intent发送以后，所有已经注册的BroadcastReceiver会检查注册时的IntentFilter是否与发送的Intent相匹配，
若匹配则就会调用BroadcastReceiver的onReceive()方法。

  所以当我们定义一个BroadcastReceiver的时候，都需要实现onReceive()方法。

  1）静态注册：在AndroidManifest.xml中用标签生命注册，并在标签内用标签设置过滤器。
  
  　　<receiver android:name="myRecevice">    //继承BroadcastReceiver，重写onReceiver方法
  　　　　<intent-filter>    
  　　　　　　<action android:name="com.dragon.net"></action> //使用过滤器，接收指定action广播
  　　　   </intent-filter>
  　　</receiver> 
  
  2）动态注册：
  
  　　IntentFilter intentFilter = new IntentFilter();
  　　intentFilter.addAction(String);   //为BroadcastReceiver指定action，使之用于接收同action的广播
      registerReceiver(BroadcastReceiver br,intentFilter);
      或者：
        registerReceiver(BroadcastReceiver br, new IntentFilter(string));
      //一般：在onStart中注册，onStop中取消unregisterReceiver

其中在动态注册中可继承BroadcastReceiver类进行封装，添加构造函数和BroadcastReceiver注册
