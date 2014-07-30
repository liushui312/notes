Android系统一共提供了四种数据存储方式。分别是：SharePreference、SQLite、Content Provider 和 File.

Content Provider    http://www.oschina.net/question/54100_34752
  Content Provider 提供了一种多应用间数据共享的方式

  标准的 Content Provider:
    1. 联系人
    2. 图片库
    3. 音乐库、视频库

  查询数据    
    URI
  修改数据    
  添加数据    
  删除数据

数据模型:
  Content Provider展示数据类似一个单个数据库表，每行有个带唯一值的数字字段，名为_ID，
可用于对表中指定记录的定位； Content provider 返回的数据结构是Cursor对象。

每个contentprovider定义一个唯一的公开的URI，用于指定到它的数据集。

1. 首先需要获得一个ContentResolver的实例，可通过Activity的成员方法getContentResovler（）方法：
    ContentResolver cr = getContentResolver();

2. 
       

-------------------------------------------------------------------------------
SharePreference
  SharedPreferences是一种轻型的数据存储方式，它的本质是基于XML文件存储key-value键值对数据，
通常用来存储一些简单的配置信息。其存储位置在/data/data/<包名>/shared_prefs目录下。
SharedPreferences对象本身只能获取数据而不支持存储和修改，存储修改是通过Editor对象实现。

实现SharedPreferences存储的步骤如下：
　　一、根据Context获取SharedPreferences对象
            getSharedPreferences(String name, int mode)方法
                name: xml文件名字
                mode: 0 or MODE_PRIVATE 默认
　　二、利用SharedPreferences对象的edit()方法获取Editor对象。
            SharedPreferences.Editor editor = sp.edit();
　　三、通过Editor对象存储key-value键值对数据。
            putString()
            putInt()
            putBoolean()
　　四、通过Editor对象的commit()方法提交数据

//获取SharedPreferences对象
    Context ctx = MainActivity.this;       
    SharedPreferences sp = ctx.getSharedPreferences("SP", MODE_PRIVATE);
//存入数据
    Editor editor = sp.edit();
    editor.putString("STRING_KEY", "string");
    editor.putInt("INT_KEY", 0);
    editor.putBoolean("BOOLEAN_KEY", true);
    editor.commit();

获取SharedPreferences数据：
    通过getXXX方法，可以方便的获得对应Key的Value值

//返回STRING_KEY的值
    Log.d("SP", sp.getString("STRING_KEY", "none"));
//如果NOT_EXIST不存在，则返回值为"none"
    Log.d("SP", sp.getString("NOT_EXIST", "none"));

------------------------------------------------------------------------------
SQLite

---------------------------------------------------------------------------------
File
存储数据: Activity提供了openFileOutput()方法可以用于把数据输出到文件中

  FileOutputStream outStream=this.openFileOutput("a.txt",Context.MODE_WORLD_READABLE);
  outStream.write(text.getText().toString().getBytes());
  outStream.close();
  Toast.makeText(MyActivity.this,"Saved",Toast.LENGTH_LONG).show();

    创建的文件保存在/data/data/<package name>/files目录
    操作模式:
      Context.MODE_PRIVATE    =  0
      Context.MODE_APPEND    =  32768
      Context.MODE_WORLD_READABLE =  1
      Context.MODE_WORLD_WRITEABLE =  2

Context.MODE_PRIVATE：为默认操作模式，代表该文件是私有数据，只能被应用本身访问，在该模式下，写入的内容会覆盖原文件的内容
Context.MODE_APPEND：模式会检查文件是否存在，存在就往文件追加内容，否则就创建新文件。
Context.MODE_WORLD_READABLE和Context.MODE_WORLD_WRITEABLE用来控制其他应用是否有权限读写该文件。
MODE_WORLD_READABLE：表示当前文件可以被其他应用读取；MODE_WORLD_WRITEABLE：表示当前文件可以被其他应用写入。

读取数据: Activity提供了openFileInput()方法
  FileInputStream inStream=this.openFileInput("a.txt");
  ByteArrayOutputStream stream=new ByteArrayOutputStream();
  byte[] buffer=new byte[1024];
  int length=-1;
  while((length=inStream.read(buffer))!=-1)   {
      stream.write(buffer,0,length);
  }
  stream.close();
  inStream.close();
  text.setText(stream.toString());
  Toast.makeText(MyActivity.this,"Loaded",Toast.LENGTH_LONG).show();

  Activity还提供了getCacheDir()和getFilesDir()方法：
    getCacheDir()方法用于获取/data/data/<package name>/cache目录
    getFilesDir()方法用于获取/data/data/<package name>/files目录

把文件放入SD卡:

  在AndroidManifest.xml中加入访问SDCard的权限如下:
    <!-- 在SDCard中创建与删除文件权限 -->
    <uses-permission android:name="android.permission.MOUNT_UNMOUNT_FILESYSTEMS"/>
    <!-- 往SDCard写入数据权限 -->
    <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"/> 

    if(Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)){
        File sdCardDir = Environment.getExternalStorageDirectory();//获取SDCard目录
        File saveFile = new File(sdCardDir, “a.txt”); //上面两句 File saveFile = new File("/sdcard/a.txt"); 
        FileOutputStream outStream = new FileOutputStream(saveFile);
        outStream.write("test".getBytes());
        outStream.close();
    } 

    
    Environment.getExternalStorageState()方法用于获取SDCard的状态，如果手机装有SDCard，
    并且可以进行读写，那么方法返回的状态等于Environment.MEDIA_MOUNTED。 


