ASoC驱动有如下三部分构成：
1.Platform
 1.1 CPU DAI (Digital Audio Interface)
   在嵌入式系统里面通常指CPU的I2S、PCM总线控制器，负责将音频数据从
 AIF FIFO搬运到CODEC（Playback的情形，Capture则方向相反）
   cpu dai通过snd_soc_register_dai()来注册
 
 1.2 DMA：负责将音频数据从userspace通过dma搬运到AIF FIFO.
   音频dma驱动通过snd_soc_register_platform()来注册.

   实现音频DMA操作函数集，具体见snd_pcm_ops结构体定义

   很多情况下，Platform特指音频dma驱动，并未包括cpu dai.
   值得留意的是：某些情形下是不需要dma操作的，比如Modem和CODEC直连，因为
 Modem本身已经把数据送到它的PCM FIFO了，这时只需启动PCM控制器把数据传输到
 CODEC即可；这种情形下，Machine驱动dai_link中需要指定.platform_name = 
 "snd-soc-dummy", 这是虚拟出来的Audio dma驱动。

2.CODEC
 
3.Machine
   指某一款机器，它把cpu dai、codec dai、modem dai各个音频接口通过定义
 dai_link链结起来，然后注册snd_soc_card。
 
   作为链结Platform和Codec的载体，它必须定义dai_link为音频物理链路选定
 Platform和Codec。处理机器特有的音频控件和音频事件，例如回放时打开外放功放。


-------------------------------------------------------------------------
转自： http://blog.csdn.net/droidphone/article/details/7165482

1.  ASoC的由来

  ASoC--ALSA System on Chip ，是建立在标准ALSA驱动层上，为了更好地支持
嵌入式处理器和移动设备中的音频Codec的一套软件体系。在ASoc出现之前，内核
对于SoC中的音频已经有部分的支持，不过会有一些局限性：
    1.Codec驱动与SoC CPU的底层耦合过于紧密，这种不理想会导致代码的重复，
      例如，仅是wm8731的驱动，当时Linux中有分别针对4个平台的驱动代码。
    2.音频事件没有标准的方法来通知用户，例如耳机、麦克风的插拔和检测，
      这些事件在移动设备中是非常普通的，而且通常都需要特定于机器的代码
      进行重新对音频路径进行配置。
    3.当进行播放或录音时，驱动会让整个codec处于上电状态，这对于PC没问题，
      但对于移动设备来说，这意味着浪费大量的电量。同时也不支持通过改变
      取样频率和偏置电流来达到省电的目的。

  ASoC正是为了解决上述种种问题而提出的，目前已经被整合至内核的代码树中：
sound/soc。ASoC不能单独存在，他只是建立在标准ALSA驱动上的一个它必须和标准
的ALSA驱动框架相结合才能工作。

2. 硬件架构

  通常，就像软件领域里的抽象和重用一样，嵌入式设备的音频系统可以被划分为
板载硬件（Machine）、Soc（Platform）、Codec 三大部分，如下图所示：

  1.Machine 是指某一款机器，可以是某款设备，某款开发板，又或者是某款智能手
机，由此可以看出Machine几乎是不可重用的，每个Machine上的硬件实现可能都不一
样，CPU不一样，Codec不一样，音频的输入、输出设备也不一样，Machine为CPU、
Codec、输入输出设备提供了一个载体。

  2.Platform 一般是指某一个SoC平台，比如,s3cxxxx,omapxxx等等，与音频相关的
通常包含该SoC中的时钟、DMA、I2S、PCM等等，只要指定了SoC，那么我们可以认为它
会有一个对应的Platform，它只与SoC相关，与Machine无关，这样我们就可以把
Platform抽象出来，使得同一款SoC不用做任何的改动，就可以用在不同的Machine中
使用。实际上，把Platform认为是某个SoC更好理解。

  3.Codec 编解码器，包含了I2S接口、D/A、A/D、Mixer、PA（功放），通常包含多
种输入（Mic、Line-in、I2S、PCM）和多个输出（耳机、喇叭、听筒，Line-out），
Codec和Platform一样，是可重用的部件，同一个Codec可以被不同的Machine使用。
嵌入式Codec通常通过I2C对内部的寄存器进行控制。

3. 软件架构

  在软件层面，ASoC也把嵌入式设备的音频系统同样分为3大部分，
Machine，Platform和Codec。

  1.Codec驱动  
  ASoC中的一个重要设计原则就是要求Codec驱动是平台无关的，它包含了一些音频的
控件（Controls），音频接口，DAMP（动态音频电源管理）的定义和某些Codec IO功
能。为了保证硬件无关性，任何特定于平台和机器的代码都要移到Platform和Machine
驱动中。所有的Codec驱动都要提供以下特性：
    Codec DAI 和 PCM的配置信息；
    Codec的IO控制方式（I2C，SPI等）；
    Mixer和其他的音频控件；
    Codec的ALSA音频操作接口；

必要时，也可以提供以下功能：
    DAPM描述信息；
    DAPM事件处理程序；
    DAC数字静音控制

  2.Platform驱动  
  它包含了该SoC平台的音频DMA和音频接口的配置和控制（I2S，PCM，AC97等等）；
它也不能包含任何与机器相关的代码。

  3.Machine驱动  
  Machine驱动负责处理机器特有的一些控件和音频事件（例如，当播放音频时，需要
先行打开一个放大器）；单独的Platform和Codec驱动是不能工作的，它必须由
Machine驱动把它们结合在一起才能完成整个设备的音频处理工作。


4. 数据结构

  整个ASoC是由一些列数据结构组成，要搞清楚ASoC的工作机理，必须要理解这一系
列数据结构之间的关系和作用，下面的关系图展示了ASoC中重要的数据结构之间的关
联方式： asoc_struct.png

  ASoC把声卡实现为一个Platform Device，然后利用Platform_device结构中的dev
字段：dev.drvdata，它实际上指向一个snd_soc_device结构。可以认为
snd_soc_device是整个ASoC数据结构的根本，由他开始，引出一系列的数据结构用于
表述音频的各种特性和功能。snd_soc_device结构引出了snd_soc_card和
soc_codec_device两个结构，然后snd_soc_card又引出了snd_soc_platform、
snd_soc_dai_link和snd_soc_codec结构。ASoC被划分为Machine、Platform和Codec三
大部分，如果从这些数据结构看来，snd_codec_device和snd_soc_card代表着Machine
驱动，snd_soc_platform则代表着Platform驱动，snd_soc_codec和soc_codec_device
则代表了Codec驱动，而snd_soc_dai_link则负责连接Platform和Codec。

  3.0中的数据结构更为合理和清晰，取消了snd_soc_device结构，直接用
snd_soc_card取代了它，并且强化了snd_soc_pcm_runtime的作用，同时还增加了另外
两个数据结构snd_soc_codec_driver和snd_soc_platform_driver，用于明确代表
Codec驱动和Platform驱动。

