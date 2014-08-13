
程序 name.java ---- 编译: javac name.java ----> 类 name.class
类 name.class 放入$CLASS_HOME  ---- 执行: java name ---> 

jar文件
jar cf myFile.jar *.class  //创建
jar uf foo.jar foo.class   //更新
jar xf myFile.jar          //解压
jar tf myFile.jar          //察看.jar中的文件

类装载器 ClassLoader
  作用：动态加载Class文件

类 .class文件
  优化
APK中.dex文件

DexClassLoader


数组：
    
------------------------------------------------------------------

一个.java文件中可以有很多类, 但public权限的类最多有一个，且文件名以此为名
