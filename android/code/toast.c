
Toast 用于向用户显示一些帮助/提示

重要方法：
    public static Toast makeText(Context context, CharSequence text, int duration) //设置提示信息的内容
    public void setGravity(int gravity, int xOffset, int yOffset) //设置提示信息的位置
    show()

eg: 
    Toast.makeText(getApplicationContext(), "默认Toast样式",
            Toast.LENGTH_SHORT).show();
    

