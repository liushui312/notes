1. 设置线程优先级 
android.os.Process.setThreadPriority （int priority）
或 android.os.Process.setThreadPriority （int tid， int priority）
　　
  priority：【-20，19】，高优先级 -> 低优先级.

可以使用已定义的：
  THREAD_PRIORITY_DEFAULT = 0
  THREAD_PRIORITY_LOWEST = 19
  THREAD_PRIORITY_BACKGROUND = 10
  THREAD_PRIORITY_FOREGROUND = -2
  THREAD_PRIORITY_DISPLAY = -4  系统显示，更新UI
  THREAD_PRIORITY_URGENT_DISPLAY = -8 系统显示，响应事件
  THREAD_PRIORITY_AUDIO = -16
  THREAD_PRIORITY_URGENT_AUDIO = -19

2. 获取u盘，分区的信息
  StatFs sf = new  StatFs(path);
  long  blockSize = sf.getBlockSize();
  long  blockCount = sf.getBlockCount();
  long size = blockSize*blockCount/1024/1024/1024; // bit: GB
