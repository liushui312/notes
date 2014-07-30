#!/bin/bash

CUR_FILE_PATH=`dirname $0`
install_dir="${HOME}/usr"

echo ""
echo "********* install jdk ***********"
#jdk_tar=`find ${CUR_FILE_PATH} -name jdk*.tar.gz`
#tar zvxf ${jdk_tar} -C ${install_dir} | awk 'BEGIN{FS="/"}{if(NR == 1) print $1}' | xargs -t -i mv ${install_dir}/{} ${install_dir}/jdk/
sudo apt-get install sun-java6-jdk

grep "JAVA_HOME" $HOME/.bashrc
ret=$?
if [ ! "$ret" -eq 0 ]; then
    echo  "\n#for java\nJAVA_HOME=${install_dir}/jdk"  >> ${HOME}/.bashrc
    echo  "CLASSPATH=\$JAVA_HOME/lib:\$JAVA_HOME/jre/lib" >> ${HOME}/.bashrc
    echo  "export JAVA_HOME CLASSPATH\nexport PATH=\$PATH:\$JAVA_HOME/bin\n" >> ${HOME}/.bashrc
    . ${HOME}/.bashrc
fi
echo "********* jdk 安装完成 ***********"


echo ""
echo "********* install eclipse ***********"
eclipse_tar=`find ${CUR_FILE_PATH} -name eclipse*.tar.gz`
tar zxf ${eclipse_tar} -C ${install_dir}

grep "ECLIPSE" $HOME/.bashrc
ret=$?
if [ ! "$ret" -eq 0 ]; then
    echo "\n#for eclipse\nECLIPSE=${install_dir}/eclipse\nexport PATH=\$PATH:\$ECLIPSE" >> ${HOME}/.bashrc
    . ${HOME}/.bashrc
fi
echo "********* 安装eclipse 成功 *************"

echo ""
echo "********* install android sdk ***********"
sdk_tar=`find ${DOC_PATH} -name android-sdk*.tgz`
tar zvxf ${sdk_tar} -C ${install_dir} | awk 'BEGIN{FS="/"}{if(NR == 1) print $1}' | xargs -t -i mv ${install_dir}/{} ${install_dir}/android-sdk/

grep "SDK" $HOME/.bashrc
ret=$?
if [ ! "$ret" -eq 0 ]; then
    echo "\n#for android-sdk\nSDK=${install_dir}/android-sdk/tools\nexport PATH=\$PATH:\$SDK" >> ${HOME}/.bashrc
    . ${HOME}/.bashrc
fi
echo "********* 安装android sdk 成功 *************"

echo ""
echo "********* install android ndk ***********"
ndk_tar=`find ${DOC_PATH} -name android-ndk*.tar.bz2`
tar jvxf ${ndk_tar} -C ${install_dir} | awk 'BEGIN{FS="/"}{if(NR == 1) print $1}' | xargs -t -i mv ${install_dir}/{} ${install_dir}/android-ndk/

grep "NDK" $HOME/.bashrc
ret=$?
if [ ! "$ret" -eq 0 ]; then
    echo "\n#for android-ndk\nNDK=${install_dir}/android-ndk/\nexport PATH=\$PATH:\$NDK" >> ${HOME}/.bashrc
    . ${HOME}/.bashrc
fi
echo "********* 安装android ndk 成功 *************"
