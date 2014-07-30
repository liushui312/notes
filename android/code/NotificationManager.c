
Notification    通知

一般来说， 一个Notification应该传送的消息包括：
  1. 一个状态条图标        
  2. 在拉伸的状态栏窗口中显示额外的信息和启动一个Application的Intent 
  3. 闪灯或LED
  4. 震动 

在状态栏(Status Bar)中，通知主要有两类: 
  1. 正在运行的事件 
  2. 通知事件 

Notification 类

常用字段:  
icon                设置图标
when                发送此通知的时间戳
contentIntent       设置PendingIntent对象，点击该通知时发送该Intent

contentView         通知在状态栏的显示View(自定义，具体请看下文) ，常与contentIntent配合使用，点击该通知后，
                    即触发contentIntent

tickerText          显示在状态栏中的文字

flags               设置flag位，例如FLAG_NO_CLEAR等
    FLAG_AUTO_CANCEL   该通知能被状态栏的清除按钮给清除掉
    FLAG_NO_CLEAR      该通知不能被状态栏的清除按钮给清除掉
    FLAG_ONGOING_EVENT 通知放置在正在运行

defaults            添加效果, eg: 对象.defaults|=DEFAULT_SOUND
    DEFAULT_ALL       使用默认字段
    DEFAULT_LIGHTS    默认闪光
    DEFAULT_SOUND     默认声音(uri，指向路径)
    DEFAULT_VIRATE    默认震动，后来得知需要添加震动权限VIBRATE： android.permission.VIBRATE
    

常用方法介绍：
1. 默认通知格式
void setLatestEventInfo(Context context, CharSequence contentTitle,
            CharSequence contentText,PendingIntent contentIntent)  

    功能： 显示在拉伸状态栏中的Notification属性，点击后将发送PendingIntent对象
    参数： 
    context          上下文环境
    contentTitle     状态栏中的大标题
    contentText      状态栏中的小标题
    contentIntent    点击后将发送PendingIntent对象

2. 自定义通知格式：
    设置Notification对象的contentView属性 

eg: 
    RemoteViews contentView = new RemoteViews(getPackageName(),
                    R.layout.custom_notification);
    contentView.setTextViewText(R.id.text, tickerText);
    mNotification.tickerText = tickerText;
    mNotification.contentView = contentView;


-----------------------------------------------------------------------------------------
NotificationManager     负责发通知、清除通知等

NotificationManager nm = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);

重要方法：
  nm.notify()



没有直接对通知进行更新，而是用了一个handler，让UI线程去更新
