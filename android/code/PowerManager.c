
http://buaadallas.blog.51cto.com/399160/376930

PowerManager    用于控制设备的电源状态的切换
  frameworks/base/core/java/android/os/PowerManager.java
  frameworks/base/services/java/com/android/server/PowerManagerService.java
  frameworks/base/core/java/android/os/Power.java  //Power与jni接口交互
  frameworks/base/core/jni/android_os_Power.cpp //PowerManager的jni接口实现

  hardware/libhardware_legacy/power/power.c //PowerManager的HAL代码，与电源管理的kernel交互

PowerManager类的重要方法：
  1. newWakeLock(int flags, String tag); //取得相应层次的锁
  参数flags说明:
    PARTIAL_WAKE_LOCK: Screen off, keyboard light off
    SCREEN_DIM_WAKE_LOCK: screen dim, keyboard light off
    SCREEN_BRIGHT_WAKE_LOCK: screen bright, keyboard light off
    FULL_WAKE_LOCK: screen bright, keyboard bright
    ACQUIRE_CAUSES_WAKEUP: 一旦有请求锁时强制打开Screen和keyboard light
    ON_AFTER_RELEASE: 在释放锁时reset activity timer

如果申请了 partial wakelock, 那么即使按 Power 键 , 系统也不会进 Sleep, 如 Music 播放时
    
  2. void goToSleep(long time); //强制设备进入Sleep状态

  3. void userActivity(long when, boolean noChangeLights);
     //User activity事件发生,设备会被切换到Full on的状态,同时Reset Screen off timer.

Note:
  1. 在使用以上函数的应用程序中 , 必须在其 Manifest.xml 文件中加入下面的权限 :
  <uses-permission android:name="android.permission.WAKE_LOCK" />
  <uses-permission android:name="android.permission.DEVICE_POWER" />
  2. 所有的锁必须成对的使用 , 如果申请了而没有及时释放会造成系统故障 . 如申请了 partial wakelock, 而没有及时释放 , 那系统就永远进不了 Sleep 模式.



eg:
    PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
    PowerManager.WakeLock wl = pm.newWakeLock(PowerManager.FULL_WAKE_LOCK, this
                        .getClass().getCanonicalName());
    wl.acquire();

    ...

    if (wl != null && wl.isHeld()) {
        wl.release();
        wl = null;
    }



