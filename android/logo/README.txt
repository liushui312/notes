
开机动画的源码：frameworks/base/cmds/bootanimation下

---------------------------------------------------------------------------


1.更改 kernel log (logo_linux_clut224.ppm)
  将新生成的logo_linux_clut224.ppm 替换/drivers/video/logo/logo_linux_clut224.ppm 文件，删除该文件夹下的logo_linux_clut224.o，编译烧录，就可以看到你所更改的图片。

注意：内核的配置，内核中除了要选中 Graphics support项下的 Bootup logo配置项外，还要选中 Console
display driver support配置项。否则kernel不会显示logo。

2.更改 android 第一个log (initlogo.rle)

将initlogo.rle拷贝到文件系统中，即将initlogo.rle拷贝到out/target/product/xxx/root/下，并替换同名文件。然后重新制作文件系统映像并烧写即可。

3.更改 android第二张图片(动画)
  开机动画文件为：/system/media/bootanimation.zip
    |-- part0/            #png 图片
    |-- desc.txt

        400 800 18  
        p 1 0 part0  
          #400 800 为图片的分辨率 400*800  
          #18为每秒显示图片的张数，也就是18fps/S  
          #p代表定义一个部分。  
          #p后面的第一个数是重复播放这一部分次数。如果这个数为0，就无限循环播放  
          #p后面第二个数是播放下一个部分前的延迟帧数  
          #字符串定义了加载文件的路径 

  zip -r -X -Zstore bootanimation part*/*.png desc.txt


4. uboot logo

此处有两种情况，一种情况是通过uboot提供的logo显示功能显示开机logo，另一种情况是通过定制显示函数来进行logo的显示。

  对于第一种情况,只需要在tools/logos目录下添加自己的logo图片xxx_logo.bmp(需要为bmp格式并且位深度为8)，
并修改tools目录下的makefile文件即可。在makefile文件中的相应位置加入如下代码：

ifeq ($(VENDOR),xxx)
LOGO_BMP= logos/xxx_logo.bmp
endif

