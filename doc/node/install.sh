#!/bin/bash

CUR_FILE_PATH=`dirname $0`
sudo tar -C /usr/local/ --strip-components=1 -zxvf $CUR_FILE_PATH/node-v0.11.9-linux-x86.tar.gz
