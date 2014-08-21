java.lang.Object
  android.content.Context
    android.content.ContextWrapper
      android.view.ContextThemeWrapper
        android.app.Activity

重要的方法：
  setRequestedOrientation();
  setContentView();             //加载布局文件
  findViewById();
  getSystemService();


Activity的生命周期(7个方法、3个阶段)
    onCreate
    onStart
    onRestart
    onResume
    onPause
    onStop
    onDestroy
--------------------------------------
开始Activity
onCreate -> onStart -> onResume

结束Activity
onPause -> onStop -> onDestroy

重新获取焦点
onRestart -> onStart -> onResume
--------------------------------------
焦点生命周期：Activity始终可见
onPause -> onResume -> onPause

生命周期：
onStart -> onRestart -> onResume -> onPause -> onStop -> onStart
--------------------------------------
Activity 之间传递数据

1. Intent

  Intent Structure
    1.action：
    ACTION_MAIN         <==> android.intent.action.MAIN
    ACTION_EDIT 
    ACTION_VIEW
    ACTION_PICK
    ACTION_GET_CONTENT
    
    2.category：类别， Gives additional information about the action.
    
    CATEGORY_LAUNCHER   <==> android.intent.category.LAUNCHER 
    CATEGORY_DEFAULT
    CATEGORY_ALTERNATIVE
    
    3.data：uri形式
        
    4.type ： Specifies an explicit type (a MIME type) of the intent data.
    
    5.component: 若以上找不到，则指定组件。
    
    6.extras：This is a Bundle of any additional information.

Intent 的构造函数
    1. Intent(String action);
    2. Intent(String action, Uri uri);
    3. Intent(Context packageContext, class);
    4. Intent(String action, Uri uri, Context packageContext, class)

eg:  通过静态变量传递数据
    //选择指定组件的方法
    Intent intent = new Intent(main.this, otherActivity.class);
    Intent.putExtra("name", "张三");
    Intent.putExtra("age","23");
    startActivity(intent);

    取数据
    在otherActivity类中：
    Intent intent = getIntent();
    String name = intent.getStringExtra("name");
    int age = intent.getStringExtra("age");

eg:  通过剪切板传递数据


========================================================================
