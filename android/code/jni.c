http://www.open-open.com/lib/view/open1324909652374.html


java 中：
1）使用System.loadLibrary()函数来引用JNI库；
2）声明调用JNI库的函数且前面添加native关键字； 


JNI C/C++代码说明：
 1）JNI_OnLoad()函数。该函数在Java程序调用System.loadLibrary()时，被调用执行，用于向JavaVM注册JNI函数等。在本例中首先通过参数JavaVM（Java虚拟机指针）获取当前应用程序所在的线程，即：JNIEnv。再通过调用 android::AndroidRuntime::registerNativeMethods()注册native实现的函数指针。
 2）JNI函数和Java调用函数的映射关系。使用JNINativeMethod将java调用的函数名与JNI实现的函数名联系在一起；
 3）JNI函数实现； 
