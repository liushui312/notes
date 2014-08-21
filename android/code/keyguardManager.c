
KeyguardManager    用于锁屏和解锁的类

通过调用Context.getSystemService(Context.KEYGUARD_SERVICE)来获取实例

内部类:  KeyguardLock
    disableKeyguard()函数来解除锁屏。
    reenableKeyguard()反解除锁屏。意思是：如果在调用disableKeyguard()函数之前是锁屏的，
那么就进行锁屏，否则不进行任何操作。当然如果之前没调用disableKeyguard()函数，也不进行任何操作。
