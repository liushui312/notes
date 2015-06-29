
java:
    1. android.media.AudioSystem：负责Audio系统的总体控制；
    2. android.media.AudioTrack：负责Audio系统的输出环节；
    3. android.media.AudioRecorder：负责Audio系统的输入环节。

jni(frameworks/base/core/jni):
    1. android_media_AudioSystem.cpp
    2. android_media_AudioTrack.cpp
    3. android_media_AudioRecord.cpp

native:
    1. AudioSystem.h libmedia/AudioSystem.cpp

iaudioflinger:
    lib/media/IAudioFlinger.cpp
通过binder 与BnAudioFlinger 服务通信
BnAudioFlinger::OnTransact 例如 ==> reply->writeInt32( setMasterMute(data.readInt32()) );

AudioFliger:
AudioFlinger作为一个service对IAudioFlinger提供服务
    例如 ==> status_t AudioFlinger::setStreamMute(...)
audioflinger的函数会去调用 hardware/libhardware/include/hardware/audio.h 里面定义的接口

libhardware:
    调用库里面的函数， 如，audio.primary.amlogic.so
    

    
    
