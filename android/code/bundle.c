http://blog.csdn.net/randyjiawenjie/article/details/6651437

key-value 对

两个activity之间的通讯可以通过bundle类来实现:
    类1：
        Bundle mBundle = new Bundle();
        mBundle.putString("Data", "data from TestBundle");

        Intent intent = new Intent(); 
        ...
        intent.putExtras(mBundle);

    类2：
        Bundle bundle = getIntent().getExtras();        
        String data = bundle.getString("Data");



