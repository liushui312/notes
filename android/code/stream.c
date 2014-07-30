

----------------------------------------------------------------------
InputStream  可读的字节流

可以读取文件、网络和内存中的字节数组中获得InputStream
1. 子类 FileInputStream
2. Socket对象的getInputStream() 和 URLConnection对象的getInputStream()
3. 子类 ByteArrayInputStream

子类 FilterInputStream
  孙类 BufferedInputStream  带有缓冲区域


通过 InputStreamReader 转成字符流 (可以指定编码格式)
一般用法：
    InputStreamReader isr = new InputStreamReader(new FileInputStream("test.txt"));
　　while((ch = isr.read())!=-1) {
        System.out.print((char)ch); 
    }

BufferedReader类
  从字符输入流中读取文本，缓冲各个字符，从而提供字符、数组和行的高效读取。

一般用法：
    BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream("test.txt")));
　　String data = null;
　　while((data = br.readLine())!=null) {
    System.out.println(data); 
    }

read(new char[1024]) 
readline()

------------------------------------------------------------------------
OutputStream  可写的字节流



------------------------------------------------------------------------
JsonWriter 编写JSON格式的数据
  1. To write arrays, first call beginArray().   endArray();
  2. To write objects, first call beginObject().  endObject(); 

一般使用：
  JsonWriter jsonWriter = new JsonWriter(new OutputStreamWriter(mClient.getOutputStream();)); //mClient 是一个Socket对象
  jsonWriter.setIndent("  ");
  jsonWriter.beginObject();
  jsonWriter.name("").value();
  ...
  jsonWriter.endObject();


    
