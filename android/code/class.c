Toast 用于向用户显示一些帮助/提示

重要方法：
    public static Toast makeText(Context context, CharSequence text, int duration) //设置提示信息的内容
    public void setGravity(int gravity, int xOffset, int yOffset) //设置提示信息的位置
    show()

eg: 
    Toast.makeText(getApplicationContext(), "默认Toast样式",
            Toast.LENGTH_SHORT).show();
    

----------------------------------------------------------------------------
PendingIntent 

  主要持有的信息是它所包装的Intent和当前App 的Context，使得外部App可以如同
当前 App一样的执行PendingIntent里的Intent，就算在执行时当前App已经不存在了，
也能通过存在PendingIntent里的 Context照样执行Intent。 

重要方法：
    PendingIntent.getBroadcast()
    PendingIntent.getActivities()
    PendingIntent.getService()

----------------------------------------------------------------------------
KeyguardManager    用于锁屏和解锁的类

通过调用Context.getSystemService(Context.KEYGUARD_SERVICE)来获取实例

内部类:  KeyguardLock
    disableKeyguard()函数来解除锁屏。
    reenableKeyguard()反解除锁屏。意思是：如果在调用disableKeyguard()函数之前是锁屏的，
那么就进行锁屏，否则不进行任何操作。当然如果之前没调用disableKeyguard()函数，也不进行任何操作。

------------------------------------------------------------------------------
Iterator    
 迭代器一般和泛型数组ArrayList，泛型链表LinkList配合使用，达到顺序遍历的效果.

使用：
 1. 定义泛型数组
    ArrayList<对象类型> mlist = new ArrayList<对象类型>;

 2. 给mlist加上迭代器，对它进行顺序遍历操作 
    Iterator<对象类型> it = mlist.iterator();

 3. 开始遍历
    while(it.hasNext()) {
        it.next()
        ...
    }

------------------------------------------------------------------------------
Method  
  反射机制, 

重要方法：
    对象.invoke(Object obj,Object args[])
    对象.setAccessible() 父类

    class类：
      getDeclaredMethod*() 获取的是类自身声明的所有方法，包含public、protected和private方法。
      getMethod*() 获取的是类的所有公有方法，这就包括自身的所有public方法，和从基类继承的、从接口实现的所有public方法。


使用：
  /** 
   ** InvokeTester.java 
   ** 版权所有(C) 2010 cuiran2001@163.com 
   ** 创建:崔冉  2010-2-9 下午02:07:13 
   **/  
   package com.timer.demo.objectsocket;  
   import java.lang.reflect.Method;  
       
   public class InvokeTester {  
       public int add(int param1, int param2) {  
           return param1 + param2;  
       }  

       public String echo(String mesg) {   
           return "echo" + mesg;  
       }  

       public static void main(String[] args) throws Exception {  
           Class classType = InvokeTester.class;  
           Object invokertester = classType.newInstance();  

           Method addMethod = classType.getMethod("add", new Class[] { int.class,  
                   int.class });  
           //Method类的invoke(Object obj,Object args[])方法接收的参数必须为对象，  
           //如果参数为基本类型数据，必须转换为相应的包装类型的对象。invoke()方法的返回值总是对象，
           //如果实际被调用的方法的返回类型是基本类型数据，那么invoke()方法会把它转换为相应的包装类型的对象，再将其返回  
           Object result = addMethod.invoke(invokertester, new Object[] {  
                   new Integer(100), new Integer(200) });  
           //在jdk5.0中有了装箱 拆箱机制 new Integer(100)可以用100来代替，系统会自动在int 和integer之间转换  
           System.out.println(result);  

           Method echoMethod = classType.getMethod("echo", new Class[] { String.class });  
           result = echoMethod.invoke(invokertester, new Object[] {"hello"});  
           System.out.println(result);  
       }  
   }
