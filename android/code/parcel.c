Android中的Parcel是什么
http://blog.csdn.net/mznewfacer/article/details/7847379

Parcel
    进程之间传递类对象, parcel 是存放读取数据的容器
binder进程间通信(IPC)
AIDL

parcel = Parcel.obtain(); //创建一个Parcel

obtain()                    获得一个新的parcel ，相当于new一个对象
dataSize()                  得到当前parcel对象的实际存储空间
dataCapacity()              得到当前parcel对象的已分配的存储空间, >=dataSize()值  (以空间换时间)
dataPostion()               获得当前parcel对象的偏移量(类似于文件流指针的偏移量)
setDataPosition()           设置偏移量
recyle()                    清空、回收parcel对象的内存
writeInt(int)               写入一个整数
writeFloat(float)           写入一个浮点数
writeDouble(double)         写入一个双精度数
writeString(string)         写入一个字符串
