KERN_DIR =/home/documents/source/kernel/linux-2.6.28

all:
	make -C $(KERN_DIR) M=`pwd` modules 

install:
	make -C $(KERN_DIR) M=`pwd` modules modules_install INSTALL_MOD_PATH=/nfs_root

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	/bin/rm -rf modules.order

obj-m	+=  .o

