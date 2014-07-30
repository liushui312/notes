Alsa Lib 

Lib 移植不需要修改一丁点儿的源码，只需要重新编译lib以支持自己的平台。

编译过程如下：
1.Configure

静态库
./configure --host=arm-none-linux-gnueabi --prefix=/home/liushui/github/out/alsa-lib/arm-linux/ --enable-static=yes --enable-shared=no --with-configdir=/data/local/share

动态库
./configure --host=arm-none-linux-gnueabi --prefix=/home/liushui/github/out/alsa-lib/arm-linux/ --enable-static=no --enable-shared=yes --with-configdir=/data/local/share

配置会先对此命令中的配置项进行检查，然后实现配置。需要注意的是：
配置如果出错，未必一定报错，而是会采用默认的配置选项。几个重要的配置项：

  1）--host   指定编译器。
  2）--prefix 指定编译后文件的安装路径，会在该目录中创建 lib 和 include 目录
  3）--with-configdir 
        指定 conf 文件的安装目录，该目录中对我们最有用的 
        alsa.conf ，此文件会被直接移植到你的目标系统中。
        它将影响 include/config.h中的 ALSA_CONFIG_DIR 目录
  4）--enable-static  [no]
     --enable-shared  [yes]  

2. make 

3. make install 
 1）install 可能会在一些非用户目录安装一些文件，因此需要 root 权限
 2）install 也会执行一些编译，因此记得保证 root 下的交叉编译器同样可用。 

4.安装到嵌入式平台
  1）lib 库文件，放在 /system/lib/ 中
  2）conf 文件，应放在 /data/local/share 中
-------------------------------------------------------------------------
alsa-lib 重要的数据结构和函数接口

snd_pcm_t                                           //pcm设备handle
snd_pcm_stream_t                                    //PCM stream (direction)
int snd_pcm_open(snd_pcm_t **pcmp, const char *name,         
         snd_pcm_stream_t stream, int mode)     //打开PCM设备，获得handle

snd_pcm_hw_params_t
int snd_pcm_hw_params_malloc(snd_pcm_hw_params_t*)          //分配
int snd_pcm_hw_params_any(snd_pcm_t*, snd_pcm_hw_params_t*) //初始化硬件结构
snd_pcm_hw_params_set_access()
snd_pcm_hw_params_set_format()
snd_pcm_hw_params_set_rate_near()
snd_pcm_hw_params_set_channels()
int snd_pcm_hw_params(snd_pcm_t*, snd_pcm_hw_params_t*)       //设置硬件
snd_pcm_hw_params_free()

int snd_pcm_prepare(snd_pcm_t*)                         //准备PCM硬件

snd_pcm_sframes_t snd_pcm_writei(snd_pcm_t *pcm,        //写PCM数据到声卡
        const void *buffer, snd_pcm_uframes_t size)

snd_pcm_sframes_t snd_pcm_readi(snd_pcm_t *pcm,              
        void *buffer, snd_pcm_uframes_t size)

int snd_pcm_close(snd_pcm_t*)

undrerun 状态：播放时，音频数据传输的不够快,导致饥饿状态发生
overrun 状态 ：录音时，音频数据来的太快,过度饱和

----------------------------------------------------------------------
typedef struct _snd_pcm snd_pcm_t
struct _snd_pcm
 type: snd_pcm_type_t
 stream: snd_pcm_stream_t 
 op_arg: snd_pcm_t *
 setup: int : 1
 ops: const snd_pcm_ops_t*
 fast_ops: const snd_pcm_fast_ops_t*
 mmap_channels: snd_pcm_channel_info_t *

要确认结构体中的回调函数，找结构体初始化；
 1. 搜索 "->ops = "
 2. 搜索 "const snd_pcm_ops_t" 找snd_pcm_ops_t的实例

----------------------------------------------------------------------
snd_pcm_open() 获得snd_pcm_t句柄 

 pcmp: snd_pcm_t**         用来保存pcm设备handle
 name: const char*         要打开的pcm设备名[default]，-D hw:0,0 plughw:0,0
 stream: snd_pcm_stream_t  
 mode: int                 开打模式
    SND_PCM_NONBLOCK           -N 
    SND_PCM_NO_AUTO_RESAMPLE   --disable-resample
    SND_PCM_NO_AUTO_CHANNELS   --disable-channels    
    SND_PCM_NO_AUTO_FORMAT     --disable-format
    SND_PCM_NO_SOFTVOL         --disable-softvol

函数分析
1.int snd_config_update(void)    刷新config配置
  1.1 snd_config_update_r(&snd_config, &snd_config_global_update, NULL);
2.snd_pcm_open_noupdate(pcmp, snd_config, name, stream, mode, 0)
  2.1 snd_config_search_definition()
  2.2 snd_config_set_hop(pcm_conf, hop)
  2.3 snd_pcm_open_conf(pcmp, name, root, pcm_conf, stream, mode)
    2.3.1 open_func = snd_dlobj_cache_get(lib, open_name,
                SND_DLSYM_VERSION(SND_PCM_DLSYM_VERSION), 1);
          //将获得lib库中_snd_pcm_xxx_open函数
static const char *const build_in_pcms[] = {
	"adpcm", "alaw", "copy", "dmix", "file", "hooks", "hw", "ladspa", 
    "lfloat", "linear", "meter", "mulaw", "multi", "null", "empty", "plug", 
    "rate", "route", "share","shm", "dsnoop", "dshare", "asym", "iec958", 
    "softvol", "mmap_emul", NULL
};

    2.3.2 执行open_func() <==> _snd_pcm_xxx_open()
           例： _snd_pcm_empty_open()
        2.3.2.1 snd_pcm_open_named_slave()
          2.3.2.1.1 snd_pcm_open_conf()   
          //重复2.3  ==> _snd_pcm_hw_open() => snd_pcm_hw_open()
            
1.snd_pcm_hw_open() 
  1.1 snd_ctl_hw_open()
  1.2 fd=snd_open_device(filename, fmode); //filename为"/dev/snd/controlC0"
  1.3 snd_pcm_hw_open_fd()  
    //对设备文件ioctl:SNDRV_PCM_IOCTL_INFO   SNDRV_PCM_IOCTL_PVERSION 
    //                SNDRV_PCM_IOCTL_TSTAMP
    1.3.1 snd_pcm_new()     //注意是lib里的函数，分配snd_pcm_t并初始化
    1.3.2 pcm->ops = &snd_pcm_hw_ops;          //初始化snd_pcm_t
          pcm->fast_ops = &snd_pcm_hw_fast_ops;
    1.3.3 snd_pcm_hw_mmap_control(pcm)
      1.3.3.1 mmap() //如果没有mmap,那么执行mmap映射内核空间驱动的声音缓冲区
      1.3.3.2 snd_pcm_set_appl_ptr()

------------------------------------------------------------------------
snd_pcm_hw_params()




函数分析
1. _snd_pcm_hw_params_internal(pcm, params);
  1.1 snd_pcm_hw_params_choose(pcm, params);
  1.2 snd_pcm_sw_params_default(pcm, &sw);
  1.3 snd_pcm_sw_params(pcm, &sw);
  1.4 snd_pcm_mmap(pcm);
    1.4.1 pcm->ops->mmap(pcm) <==> snd_pcm_hw_mmap()
    1.4.2 snd_pcm_channel_info(pcm, i);
      1.4.2.1 pcm->ops->channel_info(pcm, info) <==> snd_pcm_hw_info() 
    1.4.3 switch (i->type) //i = &pcm->mmap_channels[c] 
    // 针对映射的type，做相应的操作
    // SND_PCM_AREA_MMAP <=> mmap; SND_PCM_AREA_SHM <=> shmget shmat shmctl
    // SND_PCM_AREA_LOCAL <=> malloc

2. snd_pcm_prepare(pcm);




