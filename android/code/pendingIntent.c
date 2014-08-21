
PendingIntent 

  主要持有的信息是它所包装的Intent和当前App 的Context，使得外部App可以如同
当前 App一样的执行PendingIntent里的Intent，就算在执行时当前App已经不存在了，
也能通过存在PendingIntent里的 Context照样执行Intent。

重要方法：
    PendingIntent.getBroadcast()
    PendingIntent.getActivities()
    PendingIntent.getService()

