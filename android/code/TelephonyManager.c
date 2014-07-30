    http://www.cnblogs.com/linjiqin/archive/2011/02/26/1965682.html
  TelephonyManager类主要提供了一系列用于访问与手机通讯相关的状态和信息的get方法。
其中包括手机SIM的状态和信息、电信网络的状态及手机用户的信息。

添加权限：

对象可以通过Context.getSystemService(Context.TELEPHONY_SERVICE)方法来获得.

从TelephonyManager中实例化mTelephony

对象.getCallState(); //返回电话状态

  CALL_STATE_IDLE    无任何状态时 
  CALL_STATE_OFFHOOK 接起电话时
  CALL_STATE_RINGING 电话进来时 

通过Method 获得
    endCall()
    answerRingingCall()


PhoneStateListener 类
  监听电话状态改变 

第一：获取电话服务管理器
TelephonyManager manager = this.getSystemService(TELEPHONY_SERVICE);

第二：通过TelephonyManager注册我们要监听的电话状态改变事件。
manager.listen(new MyPhoneStateListener(), PhoneStateListener.LISTEN_CALL_STATE);

第三步：通过extends PhoneStateListener来定制自己的规则。将其对象传递给第二步作为参数

第四步：这一步很重要，那就是给应用添加权限。android.permission.READ_PHONE_STATE 


http://iaiai.iteye.com/blog/1075269
http://blog.csdn.net/hellogv/article/details/6065983
