a23音频驱动

源码位置：
1.音频驱动位置:linux-3.4/sound/soc/sunxi/
2.switch耳机检测驱动位置:linux-3.4/drivers/staging/android/switch/

5个音频设备驱动，分别为audiocodec，hdmiaudio，spdif，pcm(i2s0)，i2s(i2s1)
1个耳机检测驱动switch

  I2S, PCM 驱动,是接口驱动,适用于所有支持 I2S,PCM 接口的设备
  Audio Codec 是芯片内置硬件驱动
  耳机检测驱动,适用于所有 3 节或 4 节耳机,注意,硬件上的 layout


设备文件：/dev/snd/
    1.controlC0     用于声卡的控制，例如通道选择，混音，麦克风的控制等 
    2.pcmC0D0c      用于录音的pcm设备
    3.pcmC0D0p      用于播放的pcm设备 
    4.timer         定时器

C0D0代表的是声卡0中的设备0，pcmC0D0c的c代表capture，
pcmC0D0p的p代表playback，这些都是alsa-driver中的命名规则。

-----------------------------------------------------------------------
框架:

                         audio_hw                        android hal
    _________________________|__________ 
   |                                    |
   |         /proc/asound        Tiny_alsa/lib_audio   
   |         /sys/class/sound           |                 
   |                 |__________________|                 user space
---------------------|---------------------------------------------
   |          alsa sound core                             kernel space
   |                 |
   |                 |
   |          alsa-asoc-framework
   |                 | 
   |                 |___________________________
   |                 |              |            |
switch_drv        audiocodec    i2s0/daudio  i2s1/daudio
---|--------------------|-----------|------------|-------------------------
audio_line_hardware  speaker/mic  3g音频        蓝牙      Hardward space


  在音频框架中，audiocodec属于模拟音频部分，hdmiaudio，spdif，pcm，i2s属于
数字音频。其中audiocodec在3g音频通话中支持模拟音频通路和通话录音功能接口。
pcm,i2s都可以配置成pcm和i2s模式，代码中为了区分，一个以pcm命名，一个以i2s
命名。Hdmiaudio，spdif支持raw data模式。耳机检测驱动switch支持android标准的
耳机检测接口。

  在模拟音频驱动audiocodec中，支持ADC录音，DAC播放，模拟音频通路。支持四路
音频输入（mic1，mic2/mic3，phone in，line in），四路音频输出（phone out，
headphone，earphone，Speaker）。在硬件设计中，mic2用于耳机mic，mic3用于降噪
，mic2跟mic3不能同时使用。

-----------------------------------------------------------------------j
音频流buffer分析

  在音频ALSA的数据传输过程中，音频的传输是通过dma进行控制的。播放时，应用程
序把音频数据不断的写入dma buffer中，然后相应的音频驱动，不停地从dma buffer中
读取数据，送往音频的fifo中，完成数据的搬送。音频的录音过程跟播放过程恰恰相反
，音频驱动不断地经过adc采样，送往dma的buffer中，然后应用程序再不断地从dma的
buffer中读取音频数据。

  在ALSA中，音频数据的传输是以period为单位来进行dma传输的。当传输完一个
period，那么就会产生一个dma中断，更新hw_ptr读取指针位置。在音频数据传输开始
前，首先要申请一段dma buffer空间，等于音频buffer size的大小。
  params_buffer_size = config.period_size * config.period_count;
在alsa core中，dma buffer的地址是固定的，dma buffer的缓冲区是一段连续的地址
，有开始和结束边界，为了避免频繁的读写指针（hw_ptr,appl_ptr）互换的情况，
alsa框架中采用了扩展逻辑缓冲区的模式进行dma buffer的管理。

在snd_pcm_runtime结构中，使用了四个相关的字段来完成这个逻辑缓冲区的管理：
hw_ptr_base: snd_pcm_uframes_t
  环形缓冲区每一圈的基地址，当读写指针越过一圈后，它按buffer size进行移动；
2.snd_pcm_runtime.status->hw_ptr
  硬件逻辑位置，播放时相当于读指针，录音时相当于写指针；
3.snd_pcm_runtime.control->appl_ptr
  应用逻辑位置，播放时相当于写指针，录音时相当于读指针；
4.snd_pcm_runtime.boundary
  扩展后的逻辑缓冲区大小，通常是(2^n)*size；

  通过这几个字段，我们可以很容易地获得缓冲区的有效数据，剩余空间等信息，也可
以很容易地把当前逻辑位置映射回真实的dma buffer中。
如图5 dma音频播放dma传输数据流图所示，假设period_count==2。


